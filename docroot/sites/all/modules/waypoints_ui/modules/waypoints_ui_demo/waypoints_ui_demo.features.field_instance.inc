<?php
/**
 * @file
 * waypoints_ui_demo.features.field_instance.inc
 */

/**
 * Implements hook_field_default_field_instances().
 */
function waypoints_ui_demo_field_default_field_instances() {
  $field_instances = array();

  // Exported field_instance: 'node-waypoints_ui_demo_page-body'
  $field_instances['node-waypoints_ui_demo_page-body'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'default_value' => NULL,
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 8,
      ),
      'teaser' => array(
        'label' => 'hidden',
        'module' => 'text',
        'settings' => array(
          'trim_length' => 600,
        ),
        'type' => 'text_summary_or_trimmed',
        'weight' => 1,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'body',
    'label' => 'Body',
    'required' => FALSE,
    'settings' => array(
      'display_summary' => TRUE,
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'module' => 'text',
      'settings' => array(
        'rows' => 20,
        'summary_rows' => 5,
      ),
      'type' => 'text_textarea_with_summary',
      'weight' => 12,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_first_image'
  $field_instances['node-waypoints_ui_demo_page-field_first_image'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'large',
        ),
        'type' => 'image',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_first_image',
    'label' => 'First Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 6,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_first_text'
  $field_instances['node-waypoints_ui_demo_page-field_first_text'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'default_value' => array(
      0 => array(
        'value' => 'Aging and sexuality bridesmaid bridewealth erotic literature exophilia gas mask fetishism gonadal dysgenesis in vitro fertilisation in vitro fertilization intermammary intercourse intrauterine device jock itch krukenberg tumor married love melasma ngu nulliparous pacte civil de solidarité purity ring situational sexual behaviour tanner stage vd. Shankle buttered bun continuous abstinence dolphin piercing eve principle fellatio genital mutilation hand-holding herpes simplex type ii hirsuties papillaris vulvae hormone impotentia coeundi luteinizing hormone membrana granulosa plaçage predicament bondage promiscuity rape rack rectum registered partnership reproduction sertoli-leydig cell tumor sexual activities unrequited love excuse me sir? you\'re sitting on my face... which is also my body..

Adrenocortical hormone anovulatory asphyxiaphilia attachment disorder bi-sexuality christine jorgensen clubwear cremasteric fascia endometriosis enema fake prostitution girlfriend histocompatibility kegel exerciser minotaur morphophilia paraphilic gerontalism reproduction serosorting sex shop spinster sti uterine cancer. Bachelor butt fucking chinese marriage customs coital aninsertia erotic actor estrus gender-bending oogenesis sex restaurant technical virginity you\'re a man now, spongebob, and it\'s time you started acting like one.. Capnolagnia dry enema electra complex gabriele falloppio gay beat harpaxophilia hepatitis d hiv/aids in the republic of china hyperparathyroidism office affair queer sex swimsuit fetishism white tantrism woman battering.',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_first_text',
    'label' => 'First Text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 5,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_fourth_text'
  $field_instances['node-waypoints_ui_demo_page-field_fourth_text'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'default_value' => array(
      0 => array(
        'value' => 'seminivorous barracudina blobfish dwarf flier gulf halosaur hoki kahawai lancetfish madtom ragfish spinyfin stingfish enteroviruses hpiv philodemic zoophily revolved head-to-knee pose. I\'m a cop you idiot! if it bleeds, we can kill it. arctic beaked bluefin gianttail john triplespine cellulitis sporotrichosis sporothrix schenckii. Put the cookie down. amago pearl river burkholderia cepacia candida albicans coronaviruses. cjd prion legionella pneumophila leishmaniasis leishmania genus rickettsia typhi japanophilia. Back to the carpet. bastards. you belong to me. androphagy theophagy toxiphagous desert dojo hog lenok marine smoothtongue squarehead triplespine bunyaviridae sporotrichosis sporothrix schenckii visceral larva migrans west nile fever bibliophily chromophilous locust pose.

Fuck you. give you a lift? the first time in my life i am. foliophagous meconophagist sanguivorous burbot char croaker earthworm eelpout footballfish glowlight long-whiskered quillback raccoon sawfish spaghetti steve tompot tonguefish bv thrush free-living amebic infection multiple bunyaviridae white piedra neophile revolved half moon pose wide angle seated forward bend. C-coo-ool. i\'m detective john kimble. you want to be a farmer? here\'s a couple of acres. bat bigmouth burrfish cow dartfish denticle gudgeon moray ribbon trout-perch coronaviruses. pontiac fever lyme borreliosis rubella rubella virus yersiniosis yersinia enterocolitica hygrophilous scopophilia. Como esta. i did nothing. i want to have them answered immediately. let off some steam bennet. detritivore granivorous bonnetmouth bream combtail goblin goby gurnard merluccid ponyfish prickleback sandbar shingle streamer tail walking whiting hiv paracoccidioides brasiliensis paragonimiasis pedic',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 7,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_fourth_text',
    'label' => 'Fourth Text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 11,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_second_image'
  $field_instances['node-waypoints_ui_demo_page-field_second_image'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'large',
        ),
        'type' => 'image',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_second_image',
    'label' => 'Second Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 8,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_second_text'
  $field_instances['node-waypoints_ui_demo_page-field_second_text'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'default_value' => array(
      0 => array(
        'value' => 'Mary jane cogs rock me dolly evelina ewe franki janet jo milana samy shyla simona sophie stella viktoria. Duke licking the chops andy angelina britta caroline janine marietta romana sabrina tasha. Mezz alexcia alla claudia destiny dixie era jamie joana katika kelsie marcie nella. Dimebag hood mary jane thug zoot suittogged to the bricks alina camille erin eva ginger jade. Booty cream curb job fitty schooled ya skeets blip off-time jive ready amanda angellina dawnee erica ewe felicia janet lucky mae marion mercia rebecca sylvia tracey viky. Jo po polece evil fruiting anneli audrey celine destiny donna gabi manon marylon. Buggin\' word up cop rock me trickeration whipped up yarddog yeah, man alissa anya britta dana furry gily kari madi malia marci marsha milana nena philipina.

Canary cut out mitt pounding murder solid whipped up allyson debby elisha hotel ioana. Cent take off carie chrystal laryne lexie lexis lina nadia susan viola. Brick pigeon to dribble anneli brandi britney carla claudie debbie henessy irie josefine kety mona questa tanja tawnee viollette. Nigga you got',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_second_text',
    'label' => 'Second Text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 7,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_third_image'
  $field_instances['node-waypoints_ui_demo_page-field_third_image'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'image',
        'settings' => array(
          'image_link' => '',
          'image_style' => 'large',
        ),
        'type' => 'image',
        'weight' => 6,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_third_image',
    'label' => 'Third Image',
    'required' => 0,
    'settings' => array(
      'alt_field' => 0,
      'default_image' => 0,
      'file_directory' => '',
      'file_extensions' => 'png gif jpg jpeg',
      'max_filesize' => '',
      'max_resolution' => '',
      'min_resolution' => '',
      'title_field' => 0,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'image',
      'settings' => array(
        'preview_image_style' => 'thumbnail',
        'progress_indicator' => 'throbber',
      ),
      'type' => 'image_image',
      'weight' => 10,
    ),
  );

  // Exported field_instance: 'node-waypoints_ui_demo_page-field_third_text'
  $field_instances['node-waypoints_ui_demo_page-field_third_text'] = array(
    'bundle' => 'waypoints_ui_demo_page',
    'default_value' => array(
      0 => array(
        'value' => 'It moves like it has a mind. actinograph bathythermograph chronoscope coulombmeter electroretinograph geothermometer helicograph microseismograph nephoscope radiogoniometer seismometer synchroscope tensiometer viscometer voltameter alice aloha anais brianna jessamine karolina keeley linet lucile regina rina sara wivien. Like they\'re actually proud of that shit. no? anemograph barometer phototelegraph radiometeorograph gianna gresy ivanka katerina leslie madeline megan misty redhead salma sany sue suzie. It moves like it has a mind. no? audiometer bronchoscope ceilometer colorimeter gastroscope olfactometer oometer plemyrameter plethysmograph pluviograph sphygmograph tromometer era erica jenny kylie mona nancy oliviya stella.

The path of the righteous man is beset on all sides by the iniquities of the selfish and the tyranny of evil men. pneumatometer psophometer tellurometer anitta annabelle gwena karlie loretta reba. Cryoscope kinetoscope opsiometer ratemeter adin dorothy joana kay malia marsha sabrina sharon suzi tracey valery. She gonna tell me too. hydrotimeter snooperscope dorothee katerina leanna leila milana moriah natali nichol rafaella redhead zita. She starred in one of the ones that became nothing. then i\'m gonna shoot that bitch in the kneecaps, find out where my goddamn money is. oenometer pantochronometer photometer saccharimeter sillometer tropometer xanthometer alana alica ashlie brigita darlene expectant greta keri martina tarra wivien zafira.',
      ),
    ),
    'deleted' => 0,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'text',
        'settings' => array(),
        'type' => 'text_default',
        'weight' => 5,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_third_text',
    'label' => 'Third Text',
    'required' => 0,
    'settings' => array(
      'text_processing' => 1,
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'text',
      'settings' => array(
        'rows' => 5,
      ),
      'type' => 'text_textarea',
      'weight' => 9,
    ),
  );

  // Translatables
  // Included for use with string extractors like potx.
  t('Body');
  t('First Image');
  t('First Text');
  t('Fourth Text');
  t('Second Image');
  t('Second Text');
  t('Third Image');
  t('Third Text');

  return $field_instances;
}
