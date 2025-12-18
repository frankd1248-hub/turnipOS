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


}

std::string EditorApp::saveState() const {

}

void EditorApp::loadState(const std::string& block) {

}

void EditorApp::start(Context& ctx) {

}