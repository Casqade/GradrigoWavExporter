#include <Gradrigo/Gradrigo.hpp>

#include <windows.h>

#include <cassert>


namespace
{

using namespace Gradrigo;

using PFN_Initialize = decltype(Gradrigo::Initialize)*;

using PFN_NewInstance = decltype(Gradrigo::NewInstance)*;
using PFN_DestroyInstance = decltype(Gradrigo::DestroyInstance)*;

using PFN_GetBuffer = decltype(Gradrigo::GetBuffer)*;
using PFN_ParseString = decltype(Gradrigo::ParseString)*;
using PFN_StartVoice = decltype(Gradrigo::StartVoice)*;
using PFN_ReleaseVoice = decltype(Gradrigo::ReleaseVoice)*;
using PFN_StopVoice = decltype(Gradrigo::StopVoice)*;
using PFN_StopAllVoices = decltype(Gradrigo::StopAllVoices)*;
using PFN_GetResponseString = decltype(Gradrigo::GetResponseString)*;
using PFN_MidiNoteOn = decltype(Gradrigo::MidiNoteOn)*;
using PFN_MidiNoteOff = decltype(Gradrigo::MidiNoteOff)*;
using PFN_SetVariable = decltype(Gradrigo::SetVariable)*;
using PFN_ResetEngine = decltype(Gradrigo::ResetEngine)*;
using PFN_ReportBoxesAsJson = decltype(Gradrigo::ReportBoxesAsJson)*;

using PFN_AmIAlive = decltype(Gradrigo::AmIAlive)*;
using PFN_EnableCompletedVoicePolling = decltype(Gradrigo::EnableCompletedVoicePolling)*;
using PFN_PollCompletedVoice = decltype(Gradrigo::PollCompletedVoice)*;


static PFN_Initialize pfn_Initialize {};
static PFN_NewInstance pfn_NewInstance {};
static PFN_DestroyInstance pfn_DestroyInstance {};

static PFN_GetBuffer pfn_GetBuffer {};
static PFN_ParseString pfn_ParseString {};
static PFN_StartVoice pfn_StartVoice {};
static PFN_ReleaseVoice pfn_ReleaseVoice {};
static PFN_StopVoice pfn_StopVoice {};
static PFN_StopAllVoices pfn_StopAllVoices {};
static PFN_GetResponseString pfn_GetResponseString {};
static PFN_MidiNoteOn pfn_MidiNoteOn {};
static PFN_MidiNoteOff pfn_MidiNoteOff {};
static PFN_SetVariable pfn_SetVariable {};
static PFN_ResetEngine pfn_ResetEngine {};
static PFN_ReportBoxesAsJson pfn_ReportBoxesAsJson {};

static PFN_AmIAlive pfn_AmIAlive {};
static PFN_EnableCompletedVoicePolling pfn_EnableCompletedVoicePolling {};
static PFN_PollCompletedVoice pfn_PollCompletedVoice {};

} // namespace


namespace Gradrigo
{

bool Load( const char* path )
{
  thread_local bool initialized {};

  if ( initialized == false )
  {
    auto gradrigoDll = LoadLibrary(path);

    if ( gradrigoDll == nullptr )
      return false;


    pfn_Initialize = (PFN_Initialize) GetProcAddress(
      gradrigoDll, "Initialize" );

    pfn_NewInstance = (PFN_NewInstance) GetProcAddress(
      gradrigoDll, "NewInstance" );

    pfn_DestroyInstance = (PFN_DestroyInstance) GetProcAddress(
      gradrigoDll, "DestroyInstance" );

    pfn_GetBuffer = (PFN_GetBuffer) GetProcAddress(
      gradrigoDll, "GetBuffer" );

    pfn_ParseString = (PFN_ParseString) GetProcAddress(
      gradrigoDll, "ParseString" );

    pfn_StartVoice = (PFN_StartVoice) GetProcAddress(
      gradrigoDll, "StartVoice" );

    pfn_ReleaseVoice = (PFN_ReleaseVoice) GetProcAddress(
      gradrigoDll, "ReleaseVoice" );

    pfn_StopVoice = (PFN_StopVoice) GetProcAddress(
      gradrigoDll, "StopVoice" );

    pfn_StopAllVoices = (PFN_StopAllVoices) GetProcAddress(
      gradrigoDll, "StopAllVoices" );

    pfn_GetResponseString = (PFN_GetResponseString) GetProcAddress(
      gradrigoDll, "GetResponseString" );

    pfn_MidiNoteOn = (PFN_MidiNoteOn) GetProcAddress(
      gradrigoDll, "MidiNoteOn" );

    pfn_MidiNoteOff = (PFN_MidiNoteOff) GetProcAddress(
      gradrigoDll, "MidiNoteOff" );

    pfn_SetVariable = (PFN_SetVariable) GetProcAddress(
      gradrigoDll, "SetVariable" );

    pfn_ResetEngine = (PFN_ResetEngine) GetProcAddress(
      gradrigoDll, "ResetEngine" );

    pfn_ReportBoxesAsJson = (PFN_ReportBoxesAsJson) GetProcAddress(
      gradrigoDll, "ReportBoxesAsJson" );

    pfn_AmIAlive = (PFN_AmIAlive) GetProcAddress(
      gradrigoDll, "AmIAlive" );

    pfn_EnableCompletedVoicePolling = (PFN_EnableCompletedVoicePolling) GetProcAddress(
      gradrigoDll, "EnableCompletedVoicePolling" );

    pfn_PollCompletedVoice = (PFN_PollCompletedVoice) GetProcAddress(
      gradrigoDll, "PollCompletedVoice" );


    initialized =
      pfn_Initialize != nullptr &&
      pfn_NewInstance != nullptr &&
      pfn_DestroyInstance != nullptr &&
      pfn_GetBuffer != nullptr &&
      pfn_ParseString != nullptr &&
      pfn_StartVoice != nullptr &&
      pfn_ReleaseVoice != nullptr &&
      pfn_StopVoice != nullptr &&
      pfn_StopAllVoices != nullptr &&
      pfn_GetResponseString != nullptr &&
      pfn_MidiNoteOn != nullptr &&
      pfn_MidiNoteOff != nullptr &&
      pfn_SetVariable != nullptr &&
      pfn_ResetEngine != nullptr &&
      pfn_ReportBoxesAsJson != nullptr &&
      pfn_AmIAlive != nullptr &&
      pfn_EnableCompletedVoicePolling != nullptr &&
      pfn_PollCompletedVoice != nullptr &&
      true;
  }

  return initialized;
}


GRADRIGO_API GRADRIGO_INSTANCE_ID Initialize( int sample_rate )
{
  assert(pfn_Initialize != nullptr);
  return pfn_Initialize(sample_rate);
}

GRADRIGO_API GRADRIGO_INSTANCE_ID NewInstance( int sample_rate )
{
  assert(pfn_NewInstance != nullptr);
  return pfn_NewInstance(sample_rate);
}

GRADRIGO_API void DestroyInstance( GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_DestroyInstance != nullptr);
  return pfn_DestroyInstance(id);
}

GRADRIGO_API int GetBuffer( int num_samples, float* out, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_GetBuffer != nullptr);
  return pfn_GetBuffer(num_samples, out, id);
}

GRADRIGO_API int ParseString( const char* gradrigo_source_code, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_ParseString != nullptr);
  return pfn_ParseString(gradrigo_source_code, id);
}

GRADRIGO_API int StartVoice( const char* box_name, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_StartVoice != nullptr);
  return pfn_StartVoice(box_name, id);
}

GRADRIGO_API void ReleaseVoice( int voice_id, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_ReleaseVoice != nullptr);
  return pfn_ReleaseVoice(voice_id, id);
}

GRADRIGO_API void StopVoice( int voice_id, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_StopVoice != nullptr);
  return pfn_StopVoice(voice_id, id);
}

GRADRIGO_API void StopAllVoices( GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_StopAllVoices != nullptr);
  return pfn_StopAllVoices(id);
}

GRADRIGO_API const char* GetResponseString( int request_id, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_GetResponseString != nullptr);
  return pfn_GetResponseString(request_id, id);
}

GRADRIGO_API void MidiNoteOn( int note, int velocity, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_MidiNoteOn != nullptr);
  return pfn_MidiNoteOn(note, velocity, id);
}

GRADRIGO_API void MidiNoteOff( int note, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_MidiNoteOff != nullptr);
  return pfn_MidiNoteOff(note, id);
}

GRADRIGO_API void SetVariable( const char *variable_name, float value, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_SetVariable != nullptr);
  return pfn_SetVariable(variable_name, value, id);
}

GRADRIGO_API void ResetEngine( GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_ResetEngine != nullptr);
  return pfn_ResetEngine(id);
}

GRADRIGO_API const char* ReportBoxesAsJson( GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_ReportBoxesAsJson != nullptr);
  return pfn_ReportBoxesAsJson(id);
}

GRADRIGO_API int AmIAlive()
{
  assert(pfn_AmIAlive != nullptr);
  return pfn_AmIAlive();
}

GRADRIGO_API void EnableCompletedVoicePolling( bool enable, GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_EnableCompletedVoicePolling != nullptr);
  return pfn_EnableCompletedVoicePolling(enable, id);
}

GRADRIGO_API int PollCompletedVoice( GRADRIGO_INSTANCE_ID id )
{
  assert(pfn_PollCompletedVoice != nullptr);
  return pfn_PollCompletedVoice(id);
}

} // namespace Gradrigo
