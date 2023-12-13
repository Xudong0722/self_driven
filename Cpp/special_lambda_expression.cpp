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

/*
    Produced by complier:
    #include <iostream>
#include <functional>

class ConfigMgr
{
  
  private: 
  bool setting_mute;
  bool setting_rename;
  int * data;
  
  public: 
  inline ConfigMgr(bool _mute, bool _rename)
  : setting_mute{_mute}
  , setting_rename{_rename}
  {
    this->data = new int{722};
  }
  
  inline ~ConfigMgr() noexcept
  {
    if(nullptr != this->data) {
      delete this->data;
      this->data = nullptr;
    } 
    
  }
  
  inline void print_current_setting()
  {
    std::operator<<(std::operator<<(std::cout.operator<<(std::boolalpha), " setting - mute: ").operator<<(this->setting_mute), '\n');
    std::operator<<(std::operator<<(std::cout.operator<<(std::boolalpha), " setting - rename: ").operator<<(this->setting_rename), '\n');
    std::operator<<(std::operator<<(std::cout, "data is: ").operator<<(*this->data), '\n');
  }
  
};

int main()
{
  ConfigMgr * config_mgr = new ConfigMgr{true, false};
  std::operator<<(std::operator<<(std::cout, "The address of config_mgr is: ").operator<<(reinterpret_cast<const void *>(config_mgr)), '\n');
    
  class __lambda_31_33
  {
    public: 
    inline constexpr  void operator()() const
    {
      std::operator<<(std::cout, "[mgr()] begin\n");
      std::operator<<(std::operator<<(std::cout, "The address of config_mgr is: ").operator<<(reinterpret_cast<const void *>(config_mgr)), '\n');
      if(nullptr != config_mgr) {
        config_mgr->print_current_setting();
      } 
      
      std::operator<<(std::cout, "[mgr()] end\n");
    }
    
    private: 
    ConfigMgr * config_mgr;
    public: 
    // inline constexpr  __lambda_31_33(const __lambda_31_33 &) noexcept = default;
    // inline constexpr  __lambda_31_33(__lambda_31_33 &&) noexcept = default;
    __lambda_31_33(ConfigMgr * _config_mgr)
    : config_mgr{_config_mgr}
    {}
    
  };
  
  std::function<void ()> mgr = std::function<void ()>(__lambda_31_33{config_mgr});
  mgr.operator()();
  delete config_mgr;
  config_mgr = nullptr;
  std::operator<<(std::operator<<(std::cout, "The address of config_mgr is: ").operator<<(reinterpret_cast<const void *>(config_mgr)), '\n');
  mgr.operator()();
  return 0;
}

*/
