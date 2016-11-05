--- Drop index
DROP INDEX i_animals_texture_id;
DROP INDEX i_pets_animal_id;
DROP INDEX i_users_pet_id_photo_id;
DROP INDEX i_pets_user_id;
DROP INDEX i_ratings_user_id;
DROP INDEX i_user_cosmetics_uid_cosmid;
DROP INDEX i_friends_fromid_toid;

--- Script to drop all the tables
drop table textures cascade constraints purge;
drop table cosmetics cascade constraints purge;
drop table food cascade constraints purge;
drop table entertainments cascade constraints purge;
drop table sceneries cascade constraints purge;
drop table photos cascade constraints purge;
drop table pets cascade constraints purge;
drop table animals cascade constraints purge;
drop table users cascade constraints purge;
drop table ratings cascade constraints purge;
drop table age_types cascade constraints purge;
--- junction tables without auto generated id
drop table user_cosmetics cascade constraints purge;
drop table friends cascade constraints purge;

--- Drop sequences
drop sequence textures_id_seq;
drop sequence cosmetics_id_seq;
drop sequence food_id_seq;
drop sequence entertainments_id_seq;
drop sequence sceneries_id_seq;
drop sequence photos_id_seq;
drop sequence age_types_id_seq;
drop sequence pets_id_seq;
drop sequence animals_id_seq;
drop sequence users_id_seq;
drop sequence ratings_id_seq;

--- Drop custom type
drop type health_condition force;

