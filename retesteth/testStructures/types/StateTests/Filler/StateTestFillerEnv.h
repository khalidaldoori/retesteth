#pragma once
#include "../Base/StateTestEnvBase.h"
#include <retesteth/dataObject/DataObject.h>
using namespace dataobject;
using namespace test::teststruct;

namespace test
{
namespace teststruct
{
// Marks that Env is taken from StateTest filler
// Filler constructor of env converts all dec fields to hex
struct StateTestFillerEnv : StateTestEnvBase
{
    StateTestFillerEnv(DataObject const& _data);
    VALUE const& currentTimestamp() const override { return m_genesisTimestamp.getCContent(); }
    VALUE const& firstBlockTimestamp() const { return m_currentTimestamp.getCContent(); }

private:
    spVALUE m_genesisTimestamp;
};


}  // namespace teststruct
}  // namespace test
