#include <inttypes.h>
#include <map>
#include <memory>

using namespace std;

class Profile
{
public:
    Profile(uint64_t uuid, int type, char* data, size_t size);

    uint64_t uuid() const
    { return _uuid; }

    int type() const
    { return _type; }

private:
    uint64_t _uuid;
    int _type;   //EOT_xxx
    size_t _size;
    char* _data;
};

class ProfileCache
{
public:
    typedef shared_ptr<Profile> ProfilePtrT;

    ProfileCache();

    ProfilePtrT getProfile(uint64_t uuid);

private:
    map<uint64_t, ProfilePtrT> _cache;
};