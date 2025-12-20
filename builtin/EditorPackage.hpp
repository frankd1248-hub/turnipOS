#pragma once
#include "../app/Package.hpp"

class Context;
class App;
class SerializableApp;
class Command;
class Serializer;
class Deserializer;

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

    std::string getLine(int line) const;

    std::string getPos() const;

    std::vector<int> getCoords() const { return {x, y}; }

    void insertCharacter(char c);
    
    int countLines();

private:

    int x, y;
    std::string contents;

    int lineLength(int line);

    std::string multiply(std::string str, int amount);
};

class EditorCommand : public Command {
public:

    EditorCommand() {}
    ~EditorCommand() {}

    std::string name() const override { return "editor"; }
    std::string help() const override { return "editor <file>"; }

    void execute(Context& ctx, const std::vector<std::string>& args) override;

private:
    bool m_running;
    EditorFile m_file;
    std::filesystem::path m_path;

    void exec(Context& ctx, std::string command);

    void writeChanges();

    void display(Context& ctx, int mode, std::string cmd);
};

class EditorApp : public App, public SerializableApp {
public:

    EditorApp() : m_cmd() {}
    ~EditorApp() {}

    std::string name() const override { return "editor"; }

    std::string saveState() const override;
    void loadState(const std::string& block) override;

    void start(Context& ctx) override;

private:
    EditorCommand m_cmd;
};

class EditorPackage : public Package {
public:

    EditorPackage() {
        m_commands.push_back(std::make_unique<EditorCommand>());
        m_apps.push_back(std::make_unique<EditorApp>());
    }

    ~EditorPackage() {}

    std::string name() const override { return "editor"; }
    uint32_t stateVersion() const override { return 1; }

    void install(Context& ctx, Shell& sh) override;
    void uninstall(Context& ctx, Shell& sh) override;

    std::vector<std::unique_ptr<App>> apps() override;
    std::vector<std::unique_ptr<Command>> commands() override;

    void save(Serializer& out) const override;
    void load(Deserializer& in) override;

private:

    std::vector<std::unique_ptr<App>> m_apps{};
    std::vector<std::unique_ptr<Command>> m_commands{};
};