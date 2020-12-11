#include <cstdlib>
#include <cstddef>
#include <string>
#include <stdio.h>
#include <map>
#include <vector>
#include <assert.h>

template<typename EnumName>
class EnumInfo {
    public:
    EnumInfo(const char* initStr) {
        int32_t enumValue = -1;
        const char *c = initStr;
        const char *nameStart = nullptr;
        const char *nameEnd = nullptr;
        while(true) {
            if(std::isalnum(*c) || ((*c) == '_')) {
                if(nameStart == nullptr) {
                    // Start one
                    ++ enumValue;
                    nameStart = c;
                }
            }
            else {
                if(nameStart != nullptr && nameEnd == nullptr){
                    // End enum name
                    nameEnd = c;
                }
                if((*c) == '=') {
                    //Forward enum value
                    char *end;
                    long v = std::strtol(c+1, &end, 0);
                    if((c+1) != end) {
                        enumValue = v;
                        c = end - 1;
                    }
                }
            }

            if((*c) == ',' || (*c) == '\0') {
                if(nameEnd != nullptr) {
                    //End one
                    std::string name(nameStart, nameEnd);
                    //Add any map here 
                    if(! _valueNameMap.emplace(enumValue, name).second) {
                        assert(false);
                    }
                    printf("%s=%d \n", name.c_str(), enumValue);
                    nameStart = nullptr;
                    nameEnd = nullptr;
                }
                if((*c) == '\0') {
                    //End str
                    break;
                }
            }
            ++ c;
        }
    }
    public: 
    const char* Name(const EnumName& e) const {
        std::map<int32_t, std::string>::const_iterator iter = _valueNameMap.find((int32_t)e);
        if(iter != _valueNameMap.end()) {
            return iter->second.c_str();
        }
        return "ERROR_INVALID";
    }
    private:
    std::map<int32_t, std::string>  _valueNameMap;
};
#define ENUMCLASSV(EnumName, ...) \
    enum class EnumName \
{ \
    __VA_ARGS__ \
}; \
class EnumName##Info \
{\
    public: \
      static EnumInfo<EnumName>& Info() \
      { \
          static EnumInfo<EnumName> info(#__VA_ARGS__); \
          return info; \
      } \
      static const char* Name(const EnumName& e) \
      { \
          return Info().Name(e); \
      } \
};
ENUMCLASSV(T2,
        t2_0, 
        t2_1 =  1001, 
        t2_3 = 0x10,
        t2_4
        );
int main()
{
    printf("name=%s, %s", T2Info::Name(T2::t2_0), T2Info::Name(T2::t2_3));
    return 0;
}
