#pragma once
#include "../app/Package.hpp"

class Context;
class App;
class SerializableApp;
class Command;
class Serializer;
class Deserializer;

class EditorApp : public App, public SerializableApp {
public:

    EditorApp() {}
    ~EditorApp() {}

    std::string name() const override { return "editor"; }

    std::string saveState() const override;
    void loadState(const std::string& block) override;

    void start(Context& ctx) override;
};

class EditorCommand : public Command {
public:

    EditorCommand() {}
    ~EditorCommand() {}

    std::string name() const override { return "editor"; }
    std::string help() const override { return "editor <file>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;
};

class EditorPackage : public Package {
public:

    EditorPackage();
    ~EditorPackage() {}

    std::string name() const override { return "editor"; }
    uint32_t stateVersion() const override { return 1; }

    void install(Context& ctx) override;
    void uninstall(Context& ctx) override;

    std::vector<App*> apps() override;
    std::vector<Command*> commands() override;

    void save(Serializer& out) const override;
    void load(Deserializer& in) override;

private:
    EditorApp m_app;
    EditorCommand m_command;

    std::vector<App*> m_apps{&m_app};
    std::vector<Command*> m_commands{&m_command};
};

class EditorFile {
public:

    EditorFile(const std::string& starting) : x(0), y(0) {
        contents = starting;
    }

    EditorFile() : x(0), y(0), contents("") {}

    std::string str() const { return contents; }

    void move(int dx, int dy) {
        x += dx;
        y += dy;
        if (y > countLines()) {
            contents.append(multiply("\n", y-countLines()));
        } 
    }

private:

    int x, y;
    std::string contents;

    int countLines() {
        std::string _cont(contents);
        int lines = 0;
        while (_cont.find_first_of('\n') != std::string::npos) {
            lines++;
            _cont = _cont.substr(_cont.find_first_of('\n')+1);
        }
        return lines;
    }

    int lineLength(int line) {
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

    std::string multiply(std::string str, int amount) {
        std::string result("");
        for (int i = 0; i < amount; i++) {
            result.append(str);
        }
        return result;
    }
};