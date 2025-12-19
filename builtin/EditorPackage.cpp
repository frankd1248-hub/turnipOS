#include <filesystem>
#include <fstream>
#include <sstream>
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
    }
    
    m_running = true;

    m_path = std::filesystem::path(args.at(0));
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

    char input;
    int mode = 0;
    // mode 0 = commands
    // mode 1 = insert
    bool isTypingCommand = false;
    std::string command = "";
    
    while (m_running) {
        display(ctx, mode);
        input = getchar();
        if (mode == 0) {
            if (!isTypingCommand && input == ':') {
                isTypingCommand = true;
            } else if (isTypingCommand && input != '\n') {
                command += input;
            } else if (isTypingCommand && input == '\n') {
                exec(ctx, command);
                continue;
            }

            if (input == 'i') {
                mode = 1;
                continue;
            }
        } else if (mode == 1) {

        }
    }
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

void EditorCommand::display(Context& ctx, int mode) {
    std::vector<int> dims = getTerminalSize();
    int rows = dims.at(0);
    int cols = dims.at(1);

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

    ctx.io.writeLine(toprow);
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

int EditorFile::countLines() {
    std::string _cont(contents);
    int lines = 0;
    while (_cont.find_first_of('\n') != std::string::npos) {
        lines++;
        _cont = _cont.substr(_cont.find_first_of('\n')+1);
    }
    return lines;
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