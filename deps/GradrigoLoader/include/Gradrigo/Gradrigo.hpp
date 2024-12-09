/*
GRADRIGO
by Adam Sporka

LICENSE AND DISCLAIMER

   This is an experimental build.

   GRADRIGO IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT
   NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
   FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
   SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY
   DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
   DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE
   GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
   INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
   WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
   OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
   DAMAGE.

   Use of music rendered using Gradrigo is permitted without
   restrictions under the conditions that "made using Gradrigo"
   is mentioned in description, annotation, credits, or similar
   metadata associated with the music or the media for which the
   music was utilized.
*/

/// Since I couldn't find any official SDK for Gradrigo,
/// this file is a modified version of https://github.com/MySchoolEngine/GLEngine/blob/Audio/vendor/gradrigo/gradrigo-interface.h
/// , which was found thanks to the article https://cornercodes.com/2021/04/04/gradrigo-integration-with-fmod
/// Just in case, the Adam Sporka's LICENSE.txt from the same repository is at the top of this header
///
/// Gradrigo.hpp was tested with the following DLL:
///   https://github.com/adam-sporka/gradrigo-for-unity/blob/main/Assets/Gradrigo/Plugins/x86_64/gradrigo.dll

#include <Gradrigo/Gradrigo_export.hpp>

#ifndef GRADRIGO_API
#define GRADRIGO_API GRADRIGOLOADER_EXPORT
#endif


typedef int GRADRIGO_INSTANCE_ID;

namespace Gradrigo
{

// Load Gradrigo dll and import its functions
GRADRIGO_API bool Load( const char* path );

// Initialize the Gradrigo in "singleton" mode
GRADRIGO_API GRADRIGO_INSTANCE_ID Initialize(int sample_rate);

// Create a new instance of gradrigo for multi-gradrigo setup
GRADRIGO_API GRADRIGO_INSTANCE_ID NewInstance(int sample_rate);
// Destroy a gradrigo in multi-gradrigo setup (can't be used on singleton gradrigos)
GRADRIGO_API void DestroyInstance(GRADRIGO_INSTANCE_ID id);

GRADRIGO_API int GetBuffer(int num_samples_frames, float *out, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API int ParseString(const char* gradrigo_source_code, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API int StartVoice(const char* box_name, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void ReleaseVoice(int voice_id, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void StopVoice(int voice_id, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void StopAllVoices(GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API const char* GetResponseString(int request_id, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void MidiNoteOn(int note, int velocity, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void MidiNoteOff(int note, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void SetVariable(const char *variable_name, float value, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API void ResetEngine(GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API const char* ReportBoxesAsJson(GRADRIGO_INSTANCE_ID id = 0);

GRADRIGO_API int AmIAlive();
GRADRIGO_API void EnableCompletedVoicePolling(bool enable, GRADRIGO_INSTANCE_ID id = 0);
GRADRIGO_API int PollCompletedVoice(GRADRIGO_INSTANCE_ID id = 0);

} // namespace Gradrigo
