 #include <cstdint>

 class RandomReplacement{
    public:
    void SetWays(uint8_t ways);
    uint8_t GetVictim() const;
    private:
    uint8_t ways = 0;
 };

