namespace Experimentation
{

    class SeedGenerator
    {
    private:
        int seed;

    public:
        SeedGenerator() {
            this->seed = 0;
        }

        SeedGenerator(int startSeed) {
            this->seed = startSeed;
        }

        int GetNextSeed()
        {
            return seed++;
        }
    };
}