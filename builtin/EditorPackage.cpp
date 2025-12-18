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

EditorPackage::EditorPackage() {
    m_app = EditorApp();
    m_command = EditorCommand();
}

void EditorPackage::install(Context& ctx) {
    std::unique_ptr<EditorPackage> pkg(this);
    ctx.apps.install(std::move(pkg), ctx);
}

void EditorPackage::uninstall(Context& ctx) {
    ctx.apps.uninstall(name(), ctx);
}

std::vector<App*> EditorPackage::apps() {
    return m_apps;
}

std::vector<Command*> EditorPackage::commands() {
    return m_commands;
}

void EditorPackage::save(Serializer& out) const {

}

void EditorPackage::load(Deserializer& in) {

}

void EditorCommand::execute(Context& ctx, const std::vector<std::string>& args) {
    if (args.empty()) {
        ctx.io.writeLine("Usage: editor <file>");
    }

    std::filesystem::path path(args.at(0));

    EditorFile file;
    
    if (!std::filesystem::exists(path)) {
        try {
            std::filesystem::create_directories(path);
        } catch (std::filesystem::filesystem_error fe) {
            ctx.io.writeLine(concat("Failed to create directory: ", fe.what()));
        }
        std::ofstream out(args.at(0));
        out.close();
        file = EditorFile();
    } else {
        std::ifstream in(path);
        std::ostringstream ss;
        ss << in.rdbuf();
        file = EditorFile(ss.str());
    }

    char input;
    int mode = 0;
    // mode 0 = commands
    // mode 1 = insert
    bool isTypingCommand = false;
    std::string command = "";
    
    while (true) {
        input = getchar();
        if (!isTypingCommand && input == ':') {
            isTypingCommand = true;
        } else if (isTypingCommand && input != '\n') {
            command += input;
        } else if (isTypingCommand && input == '\n') {
            exec(command);
        }
    }
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