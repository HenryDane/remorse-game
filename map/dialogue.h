#ifndef _DIALOGUE_H
#define _DIALOGUE_H

class Script {
public:

    void execute();
};

class DialogueOption {
private:
    friend class Dialogue;
    std::string label, trigger, next;
};

class DialogueNode {
public:
    ~DialogueNode() { };

private:
    friend class Dialogue;
    std::vector<std::string> text;
    std::vector<DialogueOption> options;
    std::string speaker, init_script;
};

class Dialogue {
public:
    Dialogue(std::string path);
    ~Dialogue() { };

private:
    std::unordered_map<std::string, DialogueNode> nodes;
    std::unordered_map<std::string, Script> scripts;
};

#endif
