#include <filesystem>
#include <fstream>
#include <sstream>
#include <cctype>
#include <ncurses.h>
#include "EditorPackage.hpp"
#include "../Context.hpp"
#include "../app/App.hpp"
#include "../app/Serialization.hpp"
#include "../commands/Command.hpp"
#include "../io/IO.hpp"
#include "../Utils.hpp"
#include "../shell/Shell.hpp"

void EditorPackage::install(Context& ctx, Shell& sh) {
    std::unique_ptr<EditorPackage> pkg(this);

    for (auto& cmd : pkg.get()->commands()) {
        sh.commands().emplace(cmd.get()->name(), std::move(cmd));
    }
 }

void EditorPackage::uninstall(Context& ctx, Shell& sh) {
    ctx.apps.uninstall(name(), ctx);
}

std::vector<std::unique_ptr<App>> EditorPackage::apps() {
    return std::move(m_apps);
}

std::vector<std::unique_ptr<Command>> EditorPackage::commands() {
    return std::move(m_commands);
}

void EditorPackage::save(Serializer& out) const {

}

void EditorPackage::load(Deserializer& in) {

}

void EditorCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    if (args.empty()) {
        ctx.io.writeLine("Usage: editor <file>");
        return;
    }
    
    m_running = true;
    initscr();
    cbreak();
    noecho();

    m_path = std::filesystem::path(args.at(0).substr(0, args.at(0).find_last_of('/')));
    if (!std::filesystem::exists(m_path)) {
        try {
            std::filesystem::create_directories(m_path);
        } catch (std::filesystem::filesystem_error fe) {
            ctx.io.writeLine(concat("Failed to create directory: ", fe.what()));
            m_running = false;
            return;
        }
        std::ofstream out(args.at(0));
        out.close();
        m_file = EditorFile();
    } else {
        std::ifstream in(m_path);
        std::ostringstream ss;
        ss << in.rdbuf();
        m_file = EditorFile(ss.str());
    }

    int input;
    int mode = 0;
    // mode 0 = commands
    // mode 1 = insert
    bool isTypingCommand = false;
    std::string command = "";
    
    while (m_running) {
        display(ctx, mode, command);
        input = getch();
        if (mode == 0) {
            if (!isTypingCommand && input == ':') {
                isTypingCommand = true;
            } else if (isTypingCommand) {
                if (input == '\b' || input == 127 || input == KEY_BACKSPACE) {
                    if (command != "") {
                        command = command.substr(0, command.size()-1);
                    }
                } else if (input != '\n')
                    command += (char) input;
                else {
                    exec(ctx, command);
                    command = "";
                    continue;
                }
            }

            if (input == 'i') {
                mode = 1;
                continue;
            }
        } else if (mode == 1) {
            if (input == 27) {
                mode = 0;
                continue;
            } else if (input == '\b' || input == 127 || input == KEY_BACKSPACE) {
                m_file.deleteCharacter();
            } else {
                m_file.insertCharacter(input);
            }
        }
        sleepMillis(17);
    }

    endwin();
}

void EditorCommand::exec(Context& ctx, std::string command) {
    if (command == ":q") {
        m_running = false;
        return;
    } else if (command == ":w") {
        writeChanges();
        return;
    }
}

void EditorCommand::display(Context& ctx, int mode, std::string cmd) {
    std::vector<int> dims = getTerminalSize();
    int rows = dims.at(0);
    int cols = dims.at(1);

    clear();
    std::string toprow;
    std::string coords = "Pos: ";
    coords.append(m_file.getPos());
    if (coords.length() <= cols - 3) {
        toprow.append(coords);
        toprow.append(" | ");
    }
    std::string modeStr = (mode == 0 ? "MODE: CMD" : "MODE: INS");
    if (toprow.length() + modeStr.length() <= cols) {
        toprow.append(modeStr);
    }

    printw("%s\n", toprow.c_str());

    int x, y;
    std::vector<int> _coords = m_file.getCoords();
    x = (coords.size() >= 1 ? _coords.at(0) : 0);
    y = (coords.size() >= 2 ? _coords.at(1) : 0);

    for (int i = y; i < rows-2, i < m_file.countLines(); i++) {
        if (i < m_file.countLines()) {
            printw("~ %s\n", m_file.getLine(i).substr(x, x + cols-1).c_str());
        } else {
            printw("~\n");
        }
    }

    if (mode == 0) {
        printw("%s\n", cmd.c_str());
    }
    refresh();
}

void EditorCommand::writeChanges() {
    std::string contents = m_file.str();
    std::ofstream out(m_path);
    out << contents;
    out.close();
}

std::string EditorApp::saveState() const {
    return "";
}

void EditorApp::loadState(const std::string& block) {
    return;
}

void EditorApp::start(Context& ctx) {
    ctx.io.write("Enter path for file to edit: ");
    m_cmd.execute(ctx, {ctx.io.readLine()});
}

std::string EditorFile::getPos() const {
    return concat(
        "row: ", std::to_string(x), ", col: ", std::to_string(y)
    );
}

std::string EditorFile::getLine(int line) const {
    return splitString(contents, '\n').at(line);
} 

int EditorFile::countLines() {
    std::string _cont(contents);
    int lines = 0;
    while (_cont.find_first_of('\n') != std::string::npos) {
        lines++;
        _cont = _cont.substr(_cont.find_first_of('\n')+1);
    }
    return lines;
}

void EditorFile::insertCharacter(char c) {
    int len = 0;
    for (int i = 0; i < y; i++) {
        len += lineLength(i) - 1;
    }
    len += x;
    if (len < contents.size())
        contents.insert(len, c + "");
}

void EditorFile::deleteCharacter() {
    int len = 0;
    for (int i = 0; i < y; i++) {
        len += lineLength(i);
    }
    len += x;
    if (len < contents.size())
        contents.erase(len, 1);
}

int EditorFile::lineLength(int line) {
    std::string _cont(contents);
    for (int i = 1; i < line; i++) {
        int pos = _cont.find_first_of('\n');
        _cont = _cont.substr(pos+1);
    }
    int pos = _cont.find_first_of('\n');
    if (pos != std::string::npos) {
        _cont = _cont.substr(0, pos);
    }

    return _cont.size();
}

std::string EditorFile::multiply(std::string str, int amount) {
    std::string result("");
    for (int i = 0; i < amount; i++) {
        result.append(str);
    }
    return result;
}