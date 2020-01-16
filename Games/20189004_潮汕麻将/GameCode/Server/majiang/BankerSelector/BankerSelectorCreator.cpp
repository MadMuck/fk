#include "BankerSelectorCreator.h"

BankerSelector *BankerSelectorCreator::Create()
{
    return new BankerSelector();
}

void BankerSelectorCreator::Destroy(BankerSelector *selector)
{
    delete selector;
}
