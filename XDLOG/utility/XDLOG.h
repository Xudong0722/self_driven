namespace utility{
    namespace elvis{

        enum LOG_LEVEL{
            NONE = -1,
            DEBUG = 0,
            INFO = 1,
            WARN = 2,
            ERROR = 3,
            FATAL = 4,
            TRACE = 5,
        };

        static const char* LOG_LEVEL2STR[] = {
            "DEBUG",
            "INFO",
            "WARN",
            "ERROR",
            "FATAL",
            "TRACE",
        };

        class XDLOG{
        private:
            XDLOG();
            ~XDLOG();

        public:
            static XDLOG& GetInst();
            void log(LOG_LEVEL log_level, const char* function_name, ...);

            template<typename... Args>
            void log_test(Args ...);
        };
    }

}
