// ### Crates structs ###

typedef enum eCrateType
{
    CT_CASH,
    CT_EXPLODE,
    CT_REVEAL,
    CT_NOMAP,
    CT_UNIT,
    CT_POWERUP, // Extended crate type, originally STEALTH type
    CT_EXECUTE_EVENT, // New crate type (mod), originally UNSUPPORTED6 type
    CT_SPICE_BLOOM_SPAWNER,
    CT_SPICE_BLOOM_SMALL,
    CT_SPICE_BLOOM_MEDIUM,
    CT_SPICE_BLOOM_LARGE
}eCrateType;

typedef enum eCrateImage
{
    CI_BLUE_CRATE,
    CI_RED_CRATE,
    CI_GREEN_CRATE,
    CI_PURPLE_CRATE,
    CI_INVISIBLE,
    CI_SMALL_BLOOM,
    CI_MEDIUM_BLOOM,
    CI_LARGE_BLOOM
}eCrateImage;

typedef struct CrateStruct
{
  uint8_t __x;
  uint8_t __y;
  uint8_t __type;
  uint8_t __is_active;
  uint8_t __image;
  uint8_t __times_to_respawn;
  uint8_t ext_data_field; // Field for extended crate functionality (mod)
  uint8_t custom_byte_1; // Field for extended crate functionality (mod)
  int32_t __timing;
}CrateStruct;

#define MAX_CRATES 30
