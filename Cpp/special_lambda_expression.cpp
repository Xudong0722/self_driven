#include <iostream>
#include <functional>

class ConfigMgr{
private:
    bool setting_mute;
    bool setting_rename;
    int* data;
public:
    ConfigMgr(bool _mute = false, bool _rename = false): setting_mute(_mute), setting_rename(_rename){
        data = new int{722};
    }
    ~ConfigMgr(){
        if(nullptr != data){
            delete data;
            data = nullptr;
        }
    }

    void print_current_setting(){
        std::cout << std::boolalpha << " setting - mute: " << this->setting_mute << '\n';
        std::cout << std::boolalpha <<  " setting - rename: " << this->setting_rename << '\n';
        std::cout << "data is: " << *this->data << '\n';
    }
};

int main()
{
    ConfigMgr* config_mgr = new ConfigMgr{true, false};
    std::cout << "The address of config_mgr is: " << config_mgr << '\n';
    std::function<void()> mgr = [config_mgr](){ // Dangerous!  undefined behavior! Maybe this pointer has been deleted outside.
        std::cout << "[mgr()] begin\n";
        std::cout << "The address of config_mgr is: " << config_mgr << '\n';
        if(nullptr != config_mgr) config_mgr->print_current_setting();
        std::cout << "[mgr()] end\n";
    };

    mgr();
    delete config_mgr;
    config_mgr = nullptr;
    std::cout << "The address of config_mgr is: " << config_mgr << '\n';
    mgr();

    return 0;
}