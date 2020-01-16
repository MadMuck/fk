#ifndef BANKER_SELECTOR_CREATOR_H
#define BANKER_SELECTOR_CREATOR_H

#include "BankerSelector.h"

class BankerSelectorCreator
{
public:
    static BankerSelector *Create();
    static void Destroy(BankerSelector *seletor);
};

#endif
