#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

#define CONFIG_FILENAME "tamagochi.dat"

void debug(char *str, ...) {
    if (getenv("DEBUG") != NULL) {
        va_list args;
        va_start(args, str);

        printf("[DEBUG] ");
        vprintf(str, args);
        printf("\n");

        va_end(args);
    }
}

struct Pet {
    int version;
    int happiness;
    int satiety;
    time_t last_seen;
    char reserved[56];
};

/**
 *
 * @param pet the pet to save to config file
 * @param filename the data file to write to
 * @return 0 if every thing went well, 1 otherwise
 */
static int save_pet(const struct Pet *pet, const char *filename) {
    FILE *fd = fopen(filename, "wb");
    if (fd == NULL) {
        return 1;
    }

    if (!fwrite(pet, sizeof(struct Pet), 1, fd)) {
        fclose(fd);
        return 1;
    }

    fclose(fd);
    return 0;
}

/**
 *
 * @param pet the pet to read from file
 * @param filename the data file to read from
 * @return 0 if every thing went well, 1 otherwise
 */
static int load_pet(struct Pet *pet, const char *filename) {
    FILE *fd = fopen(filename, "rb");
    if (fd == NULL) {
        return 1;
    }

    if (!fread(pet, sizeof(struct Pet), 1, fd)) {
        fclose(fd);
        return 1;
    }

    fclose(fd);
    return 0;
}

static void display_pet(const struct Pet pet) {
    printf("Pet version: %d\n", pet.version);
    printf("Happiness: %d\n", pet.happiness);
    printf("Satiety: %d\n", pet.satiety);
    printf("Last seen: %ld\n", pet.last_seen);
}

static void init(struct Pet *pet) {
    const int lp = load_pet(pet, CONFIG_FILENAME);
    if (lp) {
        debug("Couldn't read from file %s: creating new Tamagochi\n", CONFIG_FILENAME);
        pet->happiness = 100;
        pet->satiety = 100;
        pet->version = 0;
        pet->last_seen = time(NULL);
    }
    else {
        debug("Successfully loaded pet from: %s\n", CONFIG_FILENAME);
    }

    // time calculations
    long sec_since_last_seen = time(NULL) - pet->last_seen;
    pet->last_seen = time(NULL);

    // decrease satiety
    pet->satiety -= sec_since_last_seen/1800; // -2/h
    if (pet->satiety < 0) {pet->satiety = 0;}

    // decrease happiness
    // TODO not time correlated, too much restart dependent
    if (pet->satiety < 80) {
        const int happiness_lost = (80 - pet->satiety) / 2;
        pet->happiness -= happiness_lost;
    }

}

int main() {
    printf("--------------------\n");
    struct Pet *pet = malloc(sizeof(struct Pet));

    init(pet);

    display_pet(*pet);
    save_pet(pet, CONFIG_FILENAME);

    free(pet);
    return 0;
}
