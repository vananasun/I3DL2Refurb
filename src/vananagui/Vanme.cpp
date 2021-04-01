#include <map>
#include "vananagui/VananaGUI.h"
#include "vananagui/platform/Thread.h"
#include "vananagui/Vanme.h"
#include "vananagui/base.h"

static std::map<int, VananaGUI*>* s_references = new std::map<int, VananaGUI*>;

void Van::set(VananaGUI* ref) {
    // insert or replace
    auto const result = s_references->insert(std::make_pair(Thread::GetThreadId(), ref));
    if (!result.second) result.first->second = ref;
}

VananaGUI& Van::me() {
    return *s_references->find(Thread::GetThreadId())->second;
}
