<?php
/**
 * @file
 * bassets_server_image_metadata.features.uuid_term.inc
 */

/**
 * Implements hook_uuid_features_default_terms().
 */
function bassets_server_image_metadata_uuid_features_default_terms() {
  $terms = array();

  $terms[] = array(
    'name' => 'CC BY-NC-ND 3.0',
    'description' => 'Attribution-NonCommercial-NoDerivs 3.0 Unported
',
    'format' => 'filtered_html',
    'weight' => 1,
    'uuid' => '22208340-c396-440a-9382-af476e7bff04',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(),
  );
  $terms[] = array(
    'name' => 'historical',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '285e49e7-b519-4397-bf08-f2229bb7a55e',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'event',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '286aa1a7-d23a-43d4-bf88-fe3ffb703510',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'Public Domain Mark 1.0',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 8,
    'uuid' => '2b69dbff-af0e-4609-b55c-7bac4631ccce',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/publicdomain/mark/1.0/',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'graphic',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '378c3889-579a-4b78-a4d8-64790da991f5',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'CC BY-SA 3.0',
    'description' => 'Attribution-ShareAlike 3.0 Unported',
    'format' => 'filtered_html',
    'weight' => 3,
    'uuid' => '488e7f4d-dd21-40a3-bef5-97dfd9b7b75e',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/licenses/by-sa/3.0/',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'everyday life',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4ceaa44c-c83b-424e-b44a-2cbdacb0229f',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'placard',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4f1055b0-dba0-4c33-8dd9-4b79f49b381c',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'CC BY 3.0',
    'description' => 'Attribution 3.0 Unported',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4f352700-172b-48b4-8a87-f59e1da5ca85',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/licenses/by/3.0/deed.de',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'sculpture',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '4fb462f4-60ad-4392-86cf-4be393d25bfd',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'cityscape',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '5884efb9-348b-4840-a35e-8a661e5b0533',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'reconstruction drawing',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '5d48d059-c9f5-4522-b1cb-22138acecece',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'landscape',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '80cab72a-2438-4dfe-b4d2-e93d2a6b2848',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'CC BY-ND 3.0',
    'description' => 'Attribution-NoDerivs 3.0 Unported',
    'format' => 'filtered_html',
    'weight' => 4,
    'uuid' => '8f6ecf36-be43-46a0-a191-1d945da4ef31',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/licenses/by-nd/3.0/',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'CC BY-NC-SA 3.0',
    'description' => 'Attribution-NonCommercial-ShareAlike 3.0 Unported',
    'format' => 'filtered_html',
    'weight' => 2,
    'uuid' => '9d53fdad-39d5-4feb-ba2e-a3f66d7e15b5',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/licenses/by-nc-sa/3.0/',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'caricature',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => '9f71c0fc-1587-46f1-88a4-214b4b9080e3',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'painting',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => 'bcfbfdfa-9dc2-45c8-85fb-c2c08a416412',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'people',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => 'c7faea2a-20f4-4754-8797-d55d3971cf48',
    'vocabulary_machine_name' => 'image_motive_category',
  );
  $terms[] = array(
    'name' => 'CC BY-SA 2.0',
    'description' => 'Attribution-ShareAlike 2.0 Generic ',
    'format' => 'plain_text',
    'weight' => 5,
    'uuid' => 'ca256ffe-83c0-405b-83f5-6fca751b9ab3',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/licenses/by-sa/2.0/',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'CC BY-NC 3.0',
    'description' => 'Creative Commons: Namensnennung, keine kommerzielle Nutzung',
    'format' => 'plain_text',
    'weight' => 6,
    'uuid' => 'cf637bcd-868d-415c-8220-8df8e47d9cd7',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/licenses/by-sa/3.0/',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'photography',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 0,
    'uuid' => 'cffcc8e8-40e2-40c8-a250-dcb16bdad011',
    'vocabulary_machine_name' => 'image_motive_type',
  );
  $terms[] = array(
    'name' => 'All rights reserved',
    'description' => '',
    'format' => 'filtered_html',
    'weight' => 9,
    'uuid' => 'd7351c20-37c4-4691-b21e-7722cbd64fc2',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://en.wikipedia.org/wiki/All_rights_reserved',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  $terms[] = array(
    'name' => 'CC0 1.0 Universal',
    'description' => 'Public Domain Dedication',
    'format' => 'filtered_html',
    'weight' => 7,
    'uuid' => 'dd9798e1-dbd4-4643-88e9-02f40963c322',
    'vocabulary_machine_name' => 'license',
    'field_license_url' => array(
      'und' => array(
        0 => array(
          'url' => 'http://creativecommons.org/publicdomain/zero/1.0/deed.en',
          'title' => NULL,
          'attributes' => array(),
        ),
      ),
    ),
  );
  return $terms;
}
