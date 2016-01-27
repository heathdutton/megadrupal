<?php

/**
 * Returns a list of the professions.
 *
 * @return array
 *   Every record has 3 elements: id, name, parent_id.
 */
function doccheck_get_professions() {
  static $professions = array(
    array(17, "Biochemist", 37),
    array(45, "Biological / Chemical Technical Assistant", 37),
    array(14, "Biologist", 37),
    array(1005, "Business/ Management/ Economy", 104),
    array(15, "Chemist", 37),
    array(73, "Children's Nurse", 37),
    array(4, "Dentist", 4),
    array(65, "Denturist", 37),
    array(67, "Diabetes advisor", 37),
    array(35, "Emergency Medical Technician [EMT]", 37),
    array(34, "Emergency Medical Technician, Intermediate [EMT-I]", 37),
    array(18, "Ergotherapist", 37),
    array(11, "Geriatric Nurse", 37),
    array(19, "Healer", 37),
    array(74, "Hospital manager / administr.", 37),
    array(38, "Insurance Company Employee", 37),
    array(42, "Lawyer", 37),
    array(13, "Librarian", 37),
    array(7, "Management", 104),
    array(36, "Management Consultant", 37),
    array(51, "Market Researcher", 104),
    array(46, "Medical Dealer", 37),
    array(41, "Medical Documentalist", 37),
    array(22, "Medical Information Scientist", 37),
    array(23, "Medical Journalist", 37),
    array(28, "Medical Laboratory Assistant", 37),
    array(16, "Medical Physicist", 37),
    array(24, "Medical Technican", 37),
    array(20, "Midwife", 37),
    array(10, "Nurse Practitioner", 37),
    array(21, "Nursing", 37),
    array(39, "Nutrition assistant", 37),
    array(53, "Optician, Optometrist", 37),
    array(1008, "Other", 104),
    array(49, "Other industry", 104),
    array(37, "Other Medical Professions", 37),
    array(30, "Pharmaceutical Advertising Agency Employee", 37),
    array(26, "Pharmaceutical-Commercial Employee", 37),
    array(25, "Pharmaceutical-Technical Assistant", 37),
    array(2, "Pharmacist", 2),
    array(27, "Pharmacy Engineer", 37),
    array(12, "Pharmacy technician", 37),
    array(1004, "Physical scientist", 104),
    array(1, "Physician", 1),
    array(33, "Physiotherapist", 37),
    array(3006, "Podiatrist", 37),
    array(48, "Product Manager", 104),
    array(57, "Psychological Technical Assistant", 37),
    array(31, "Psychologist", 37),
    array(32, "Psychotherapist", 37),
    array(50, "Public Health Administration", 37),
    array(9, "Public Relations", 104),
    array(29, "Publishing Company Employee", 37),
    array(52, "Research Manager", 104),
    array(47, "Sales Representative", 104),
    array(6, "Scientific Director", 104),
    array(63, "Speech Therapist", 37),
    array(5, "Student", 5),
    array(54, "Student of Animal Health", 5),
    array(106, "Student of Dentistry", 5),
    array(5011, "Student of Ecotrophology", 5),
    array(105, "Student of Human Medicine", 5),
    array(43, "Student of Osteopathy", 37),
    array(56, "Student of Pharmacy", 5),
    array(1007, "Technician", 104),
    array(44, "Toxicologist", 37),
    array(3, "Veterinary", 3),
    array(1002, "Pharmacist (nl)", 1002),
  );

  return $professions;
}

/**
 * Returns a list of the disciplines.
 *
 * @return array
 *   Every record has 3 elements: id, name, parent_id.
 */
function doccheck_get_disciplines() {
  static $disciplines = array(
    array(3, "Anatomy", 3),
    array(2, "Anesthesiology", 2),
    array(1029, "Animal Feeding", 64),
    array(1030, "Animal Hygiene", 64),
    array(1032, "Animal Testing", 1032),
    array(6, "Biochemistry", 6),
    array(1026, "Cattle", 1026),
    array(39, "Clinical Chemistry", 39),
    array(38, "Clinical Pharmacology", 38),
    array(66, "Dentistry (general)", 66),
    array(19, "Dermatology", 19),
    array(71, "Disaster medicine", 71),
    array(47, "Emergency Medicine", 2),
    array(1040, "Executive Board / Management", 1040),
    array(1007, "Fish", 1007),
    array(1014, "Food", 64),
    array(59, "Forensic Medicine", 59),
    array(1, "General Medicine", 1),
    array(1021, "Horses - Internal Medicine", 1019),
    array(1020, "Horses - Surgery", 1019),
    array(1019, "Horses (general)", 1019),
    array(20, "Human Genetics", 20),
    array(21, "Hygienics und Clinical Ecology", 21),
    array(35, "Intensive Care Medicine", 2),
    array(23, "Internal Medicine -  Angiology", 22),
    array(29, "Internal Medicine - Cardiology", 22),
    array(24, "Internal Medicine - Diabetology", 22),
    array(25, "Internal Medicine - Endokrinology", 22),
    array(26, "Internal Medicine - Gastroentrology", 22),
    array(17, "Internal Medicine - Geriatry", 22),
    array(27, "Internal Medicine - Hematology", 22),
    array(28, "Internal Medicine - Immunology", 22),
    array(70, "Internal Medicine - Infection Diseases", 22),
    array(31, "Internal Medicine - Nephrology", 22),
    array(32, "Internal Medicine - Oncology", 22),
    array(33, "Internal Medicine - Pulmonology", 22),
    array(34, "Internal Medicine - Rheumatology", 22),
    array(22, "Internal Medicine (general)", 22),
    array(1013, "Laboratory diagnostics", 39),
    array(1008, "Meat Hygiene", 1008),
    array(40, "Medical Informatics", 40),
    array(41, "Microbiology", 41),
    array(43, "Naturophatic Medicine", 43),
    array(45, "Neurology (general)", 45),
    array(46, "Neuropathology", 53),
    array(44, "Neurosurgery", 7),
    array(48, "Nuclear Medicine", 48),
    array(16, "Obstetrics and Gynecology", 16),
    array(4, "Occupational Medicine", 4),
    array(5, "Ophthalmology", 5),
    array(42, "Oral and Maxillofacial Surgery", 42),
    array(1004, "Oral surgery", 66),
    array(102, "Orthodontics", 102),
    array(52, "Orthopedics", 52),
    array(1047, "Other", 1047),
    array(18, "Otolaryngology", 18),
    array(53, "Pathology (general)", 53),
    array(36, "Pediatrics (general)", 36),
    array(54, "Pharmacology/ Toxicology", 38),
    array(1024, "Physiology, Physiology Chemistry", 1024),
    array(1028, "Pigs", 1028),
    array(1009, "Poultry", 1009),
    array(1042, "Product or project manager", 1042),
    array(56, "Psychiatry/ Psychotherapy", 56),
    array(15, "Radiology (general)", 15),
    array(1043, "Regional Sales Manager", 1043),
    array(60, "Rehabilitation Medicine", 60),
    array(1025, "Reproductive Medicine", 1025),
    array(1044, "Sales Representative", 1044),
    array(1027, "Sheep and Goats", 1027),
    array(1011, "Small animals - Surgery", 1010),
    array(1010, "Small animals (general)", 1010),
    array(13, "Surgery - Accident Surgery", 7),
    array(9, "Surgery - Heart Surgery", 7),
    array(10, "Surgery - Pediatric Surgery", 7),
    array(11, "Surgery - Plastic Surgery", 7),
    array(12, "Surgery - Thorax Surgery", 7),
    array(14, "Surgery - Visceral Surgery", 7),
    array(7, "Surgery (general)", 7),
    array(1031, "Tropical Veterinary Medicine", 64),
    array(65, "Urology", 65),
    array(64, "Veterinary medicine (general)", 64),
    array(1033, "Virology", 1033),
    array(1045, "Webmaster / IT", 1045),
    array(50, "Without speciality", 50),
    array(37, "Youth and Child Psychiatry", 56),
    array(1034, "Zoo and Wild Animals", 1034),
  );

  return $disciplines;
}

/**
 * Returns a list of the countries.
 *
 * @return array
 *   Every record has 3 elements: id, name, ISO code.
 */
function doccheck_get_countries() {
  static $countries = array(
    array(118, "Afghanistan", "af"),
    array(1, "Albania", "al"),
    array(82, "Algeria", "dz"),
    array(-99, "American samoa", "as"),
    array(119, "Andorra", "ad"),
    array(120, "Angola", "ao"),
    array(-99, "Anguilla", "ai"),
    array(-99, "Antarctica", "aq"),
    array(121, "Antigua and Barbuda", "ag"),
    array(2, "Argentina", "ar"),
    array(80, "Armenia", "am"),
    array(300, "Aruba", "aw"),
    array(3, "Australia", "au"),
    array(78, "Austria", "at"),
    array(124, "Azerbaijan", "az"),
    array(123, "Bahamas", "bs"),
    array(126, "Bahrain", "bh"),
    array(127, "Bangladesh", "bd"),
    array(128, "Barbados", "bb"),
    array(105, "Belarus", "by"),
    array(4, "Belgium", "be"),
    array(129, "Belize", "bz"),
    array(130, "Benin", "bj"),
    array(-99, "Bermuda", "bm"),
    array(131, "Bhutan", "bt"),
    array(5, "Bolivia", "bo"),
    array(6, "Bosnia-Herzegovina", "ba"),
    array(133, "Botswana", "bw"),
    array(-99, "Bouvet Island", "bv"),
    array(7, "Brazil", "br"),
    array(-99, "British Indian Ocean Terr.", "io"),
    array(135, "Brunei Darussalam", "bn"),
    array(8, "Bulgaria", "bg"),
    array(136, "Burkina faso", "bf"),
    array(134, "Burundi", "bi"),
    array(100, "Cambodia", "kh"),
    array(148, "Cameroon", "cm"),
    array(9, "Canada", "ca"),
    array(37, "Cape Verde", "cv"),
    array(-99, "Cayman Islands", "ky"),
    array(195, "Central African Republic", "cf"),
    array(189, "Chad", "td"),
    array(10, "Chile", "cl"),
    array(11, "China", "cn"),
    array(-99, "Christmas Island", "cx"),
    array(-99, "Cocos (Keeling) Islands", "cc"),
    array(12, "Colombia", "co"),
    array(153, "Comoros", "km"),
    array(155, "Congo", "cg"),
    array(154, "Congo - Democratic Republic", "cd"),
    array(-99, "Cook Islands", "ck"),
    array(13, "Costa Rica", "cr"),
    array(14, "Croatia", "hr"),
    array(15, "Cuba", "cu"),
    array(77, "Cyprus", "cy"),
    array(16, "Czech Republic", "cz"),
    array(17, "Denmark", "dk"),
    array(137, "Djibouti", "dj"),
    array(138, "Dominica", "dm"),
    array(106, "Dominican Republic", "do"),
    array(-99, "East Timor", "tp"),
    array(19, "Ecuador", "ec"),
    array(20, "Egypt", "eg"),
    array(21, "El Salvador", "sv"),
    array(122, "Equatorial Guinea", "gq"),
    array(140, "Eritrea", "er"),
    array(23, "Estonia", "ee"),
    array(125, "Ethiopia", "et"),
    array(-99, "Falkland Islands (Malvinas)", "fk"),
    array(-99, "Faroe Islands", "fo"),
    array(24, "Fiji", "fj"),
    array(25, "Finland", "fi"),
    array(26, "France", "fr"),
    array(-99, "French Guiana", "gf"),
    array(-99, "French Polynesia", "pf"),
    array(-99, "French Southern Territories", "tf"),
    array(243, "F.Y.R. of Macedonia", ""),
    array(142, "Gabon", "ga"),
    array(91, "Gambia", "gm"),
    array(84, "Georgia", "ge"),
    array(18, "Germany", "de"),
    array(141, "Ghana", "gh"),
    array(-99, "Gibraltar", "gi"),
    array(27, "Greece", "gr"),
    array(-99, "Greenland", "gl"),
    array(115, "Grenada", "gd"),
    array(-99, "Guadeloupe", "gp"),
    array(-99, "Guam", "gu"),
    array(97, "Guatemala", "gt"),
    array(144, "Guinea", "gn"),
    array(145, "Guinea-Bissau", "gw"),
    array(146, "Guyana", "gy"),
    array(147, "Haiti", "ht"),
    array(-99, "Heard Island/ Mcdonald Isl.", "hm"),
    array(194, "Holy See/ Vatican City State", "va"),
    array(112, "Honduras", "hn"),
    array(28, "Hong Kong", "hk"),
    array(29, "Hungary", "hu"),
    array(30, "Iceland", "is"),
    array(31, "India", "in"),
    array(32, "Indonesia", "id"),
    array(-1, "International", "com"),
    array(103, "Iran", "ir"),
    array(103, "Iran, Islamic Republic of", "ir"),
    array(196, "Iraq", "iq"),
    array(33, "Ireland", "ie"),
    array(34, "Israel", "il"),
    array(35, "Italy", "it"),
    array(139, "Ivory Coast", "ci"),
    array(117, "Jamaica", "jm"),
    array(36, "Japan", "jp"),
    array(93, "Jordan", "jo"),
    array(92, "Kazakstan", "kz"),
    array(150, "Kenya", "ke"),
    array(152, "Kiribati", "ki"),
    array(90, "Korea, Democratic PR", "kp"),
    array(38, "Korea, Republic of", "kr"),
    array(65, "Kuwait", "kw"),
    array(151, "Kyrgyzstan", "kg"),
    array(156, "Lao People's Dem. Rep.", "la"),
    array(39, "Latvia", "lv"),
    array(102, "Lebanon", "lb"),
    array(157, "Lesotho", "ls"),
    array(158, "Liberia", "lr"),
    array(88, "Libyan Arab Jamahiriya", "ly"),
    array(81, "Liechtenstein", "li"),
    array(40, "Lithuania", "lt"),
    array(41, "Luxembourg", "lu"),
    array(-99, "Macau", "mo"),
    array(42, "Macedonia, FYR of", "mk"),
    array(159, "Madagascar", "mg"),
    array(160, "Malawi", "mw"),
    array(43, "Malaysia", "my"),
    array(161, "Maldives", "mv"),
    array(162, "Mali", "ml"),
    array(83, "Malta", "mt"),
    array(165, "Marshall Islands", "mh"),
    array(-99, "Martinique", "mq"),
    array(164, "Mauritania", "mr"),
    array(166, "Mauritius", "mu"),
    array(-99, "Mayotte", "yt"),
    array(44, "Mexico", "mx"),
    array(167, "Micronesia", "fm"),
    array(163, "Moldova, Republic of", "md"),
    array(168, "Monaco", "mc"),
    array(96, "Mongolia", "mn"),
    array(-99, "Montserrat", "ms"),
    array(87, "Morocco", "ma"),
    array(45, "Mozambique", "mz"),
    array(132, "Myanmar", "mm"),
    array(95, "Namibia", "na"),
    array(181, "Nauru", "nr"),
    array(47, "Nepal", "np"),
    array(46, "Netherlands", "nl"),
    array(-99, "Netherlands Antilles", "an"),
    array(-99, "New Caledonia", "nc"),
    array(48, "New Zealand", "nz"),
    array(170, "Nicaragua", "ni"),
    array(171, "Niger", "ne"),
    array(49, "Nigeria", "ng"),
    array(-99, "Niue", "nu"),
    array(-99, "Norfolk Island", "nf"),
    array(-99, "Northern Mariana Islands", "mp"),
    array(50, "Norway", "no"),
    array(173, "Oman", "om"),
    array(51, "Pakistan", "pk"),
    array(175, "Palau", "pw"),
    array(107, "Palestinian Territory", "ps"),
    array(79, "Panama", "pa"),
    array(177, "Papua New Guinea", "pg"),
    array(52, "Paraguay", "py"),
    array(53, "Peru", "pe"),
    array(54, "Philippines", "ph"),
    array(-99, "Pitcairn", "pn"),
    array(55, "Poland", "pl"),
    array(56, "Portugal", "pt"),
    array(-99, "Puerto Rico", "pr"),
    array(109, "Qatar", "qa"),
    array(-99, "Reunion", "re"),
    array(57, "Romania", "ro"),
    array(58, "Russian Federation", "ru"),
    array(143, "Rwanda", "rw"),
    array(-99, "S. Georgia/Sandwich Islands", "gs"),
    array(-99, "Saint Helena", "sh"),
    array(169, "Saint Kitts and Nevis", "kn"),
    array(110, "Saint Lucia", "lc"),
    array(-99, "Saint Pierre and Miquelon", "pm"),
    array(113, "Samoa", "ws"),
    array(176, "San Marino", "sm"),
    array(178, "Sao Tome and Principe", "st"),
    array(59, "Saudi Arabia", "sa"),
    array(179, "Senegal", "sn"),
    array(245, "Serbia and Montenegro", ""),
    array(180, "Seychelles", "sc"),
    array(183, "Sierra Leone", "sl"),
    array(61, "Singapore", "sg"),
    array(63, "Slovakia", "sk"),
    array(62, "Slovenia", "si"),
    array(174, "Solomon Islands", "sb"),
    array(184, "Somalia", "so"),
    array(64, "South Africa", "za"),
    array(22, "Spain", "es"),
    array(104, "Sri Lanka", "lk"),
    array(172, "St. Vincent / Grenadines", "vc"),
    array(86, "Sudan", "sd"),
    array(98, "Suriname", "sr"),
    array(-99, "Svalbard and Jan Mayen", "sj"),
    array(185, "Swaziland", "sz"),
    array(66, "Sweden", "se"),
    array(60, "Switzerland", "ch"),
    array(111, "Syrian Arab Republic", "sy"),
    array(89, "Taiwan, Province of China", "tw"),
    array(67, "Tajikistan", "tj"),
    array(186, "Tanzania, United Republic of", "tz"),
    array(69, "Thailand", "th"),
    array(187, "Togo", "tg"),
    array(-99, "Tokelau", "tk"),
    array(188, "Tonga", "to"),
    array(70, "Trinidad and Tobago", "tt"),
    array(114, "Tunisia", "tn"),
    array(71, "Turkey", "tr"),
    array(190, "Turkmenistan", "tm"),
    array(-99, "Turks and Caicos Islands", "tc"),
    array(191, "Tuvalu", "tv"),
    array(192, "Uganda", "ug"),
    array(85, "Ukraine", "ua"),
    array(73, "United Arab Emirates", "ae"),
    array(74, "United Kingdom", "gb"),
    array(72, "United States of America", "us"),
    array(68, "Uruguay", "uy"),
    array(226, "US Min. Outl. Islands", "um"),
    array(94, "Uzbekistan", "uz"),
    array(193, "Vanuatu", "vu"),
    array(108, "Venezuela", "ve"),
    array(75, "Viet Nam", "vn"),
    array(232, "Virgin Islands, British", "vg"),
    array(233, "Virgin Islands, U.S.", "vi"),
    array(234, "Wallis and Futuna", "wf"),
    array(235, "Western Sahara", "eh"),
    array(116, "Yemen", "ye"),
    array(182, "Zambia", "zm"),
    array(149, "Zimbabwe", "zw"),
  );

  return $countries;
}

/**
 * Returns a list of activities.
 *
 * @return array
 *   Every record has 2 elements: activity id, activity name
 */
function doccheck_get_activities() {
  static $activities = array(
    array(7, 'Administration'),
    array(1019, 'Advertising Agency'),
    array(1002, 'Armed Forces'),
    array(1007, 'CEO'),
    array(1016, 'Clinical pharmacy'),
    array(1010, 'Clinical Research/Laboratory'),
    array(1012, 'CRM/Customer Service'),
    array(1018, 'Doctor\'s office'),
    array(1013, 'Drug safety'),
    array(1021, 'Health Insurance Fund (private)'),
    array(1020, 'Health Insurance Fund (public)'),
    array(2, 'Hospital'),
    array(14, 'Hospital - Attending Physician'),
    array(1032, 'Hospital - CEO'),
    array(13, 'Hospital - Chief Physician'),
    array(15, 'Hospital - Physician Assistant'),
    array(1008, 'Internet/ IT/ eBusiness/EDP'),
    array(1023, 'Large animal practice'),
    array(1014, 'Management'),
    array(1005, 'Market Research'),
    array(1003, 'Marketing and Sales'),
    array(1004, 'Medical Scientist'),
    array(11, 'Other'),
    array(5, 'Other Industry'),
    array(10, 'Parental leave'),
    array(3, 'Pharmaceutical Industry'),
    array(1001, 'Practice - Employee'),
    array(1033, 'Practice - Group practice'),
    array(1000, 'Practice - Group practice (employee)'),
    array(4, 'Practice - Group practice (owner)'),
    array(1, 'Practice - Owner'),
    array(1017, 'Press/Media'),
    array(12, 'Public Pharmacy'),
    array(1009, 'Public Relations'),
    array(1011, 'Quality Assurance/Controlling'),
    array(9, 'Retired'),
    array(1006, 'Sales Representative'),
    array(6, 'Science/Research/Education'),
    array(1022, 'Small animal practice'),
    array(8, 'Unemployed'),
    array(1015, 'University/Clinical research centre'),
    array(1029, 'Independant pharmacist'),
    array(1030, 'Pharmacist employee'),
    array(21, 'Adjunct Pharmacist'),
    array(16, 'Replacement pharmacist in a public pharmacy'),
    array(19, 'Pharmacy owner (pharmacist)'),
    array(20, 'Responsible pharmacist (no owner)'),
  );

  return $activities;
}

/**
 * Returns profile categories.
 *
 * @return array
 */
function doccheck_get_profile_categories($translate = TRUE) {
  $t = get_t();

  $pi = 'Personal information';
  $p = 'Professional information';
  if ($translate) {
    $pi = $t($pi);
    $p = $t($p);
  }

  return array($pi, $p);
}

/**
 * Helper function for doccheck_install().
 *
 * Additionally, the keys of the returned array are the list of the accepted
 * properties from doccheck.
 */
function doccheck_get_profile_field_stubs() {
  $t = get_t();

  list($pi, $p) = doccheck_get_profile_categories();

  $professions = array();
  $disciplines = array();
  $activities = array();

  foreach (doccheck_get_professions() as $professiondata) {
    list($id, $prof) = $professiondata;
    $professions[$id] = $prof;
  }

  foreach (doccheck_get_disciplines() as $disciplinedata) {
    list($id, $disc) = $disciplinedata;
    $disciplines[$id] = $disc;
  }

  foreach (doccheck_get_activities() as $activitydata) {
    list($id, $act) = $activitydata;
    $activities[$id] = $act;
  }

  unset($id);
  unset($disc);
  unset($prof);
  unset($professiondata);
  unset($disciplinedata);

  // Install fields to the profile table.
  // Note: non-english names are taken from:
  // http://www.doccheck.com/fileadmin/user_upload/dc_Lounge/com/DocCheck_Login_Manual_12.1.pdf
  return array(
    'dc_anrede' => array(
      'title' => $t('Address'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 2,
      'options' => array(),
    ),
    'dc_gender' => array(
      'title' => $t('Gender'),
      'category' => $pi,
      'type' => 'selection',
      'visibility' => 2,
      'options' => array(
        $t('Male'),
        $t('Female'),
      ),
    ),
    'dc_titel' => array(
      'title' => $t('Title'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 2,
      'options' => array(),
    ),
    'dc_vorname' => array(
      'title' => $t('First name'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 2,
      'options' => array(),
    ),
    'dc_name' => array(
      'title' => $t('Last name'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 2,
      'options' => array(),
    ),
    'dc_strasse' => array(
      'title' => $t('Street'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 1,
      'options' => array(),
    ),
    'dc_plz' => array(
      'title' => $t('ZIP code'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 1,
      'options' => array(),
    ),
    'dc_ort' => array(
      'title' => $t('City'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 1,
      'options' => array(),
    ),
    'dc_land' => array(
      'title' => $t('Country'),
      'category' => $pi,
      'type' => 'textfield',
      'visibility' => 1,
      'options' => array(),
    ),
    'dc_beruf' => array(
      'title' => $t('Profession'),
      'category' => $p,
      'type' => 'selection',
      'visibility' => 2,
      'options' => $professions,
      'altnames' => array('dc_profession_id'),
    ),
    'dc_fachgebiet' => array(
      'title' => $t('Discipline'),
      'category' => $p,
      'type' => 'selection',
      'visibility' => 2,
      'options' => $disciplines,
    ),
    'dc_activity' => array(
      'title' => $t('Activity'),
      'category' => $p,
      'type' => 'selection',
      'visibility' => 2,
      'options' => $activities,
    ),
  );
}

/**
 * Returns the list of the languages supported by DocCheck.
 *
 * @return array
 */
function doccheck_get_languages() {
  static $doccheck_languages = NULL;

  if ($doccheck_languages === NULL) {
    $doccheck_languages = array(
      'com' => t('English'),
      'de' => t('German'),
      'fr' => t('French'),
      'it' => t('Italian'),
      'es' => t('Espanol'),
      'nl' => t('Dutch'),
      'befr' => t('French (Belgium)'),
      'benl' => t('Dutch (Belgium)'),
    );
  }

  return $doccheck_languages;
}

/**
 * Determines the appropriate language based on Drupal's language settings.
 *
 * @param mixed $default
 *   Default value if Drupal's language settings can't be mapped to the
 *   DocCheck languages.
 * @param bool $reset
 *   Optional parameter to reset the internal cache.
 *
 * @return mixed|string
 *   Returns the DocCheck language name if the default language can be mapped
 *   to the DocCheck language list, $default otherwise.
 */
function doccheck_get_language($default = NULL, $reset = FALSE) {
  global $language;
  static $lang = NULL;

  if ($reset) {
    $lang = NULL;
  }

  if ($lang === NULL) {
    $l = $language->language;
    if ($l == 'en') {
      $l = 'com';
    }

    $available_languages = array();
    foreach (array_keys(doccheck_get_languages()) as $code) {
      if (variable_get("doccheck_keynumber_{$code}")) {
        $available_languages[] = $code;
      }
    }

    if (in_array($l, $available_languages)) {
      $lang = $l;
    }
    foreach ($available_languages as $code) {
      if (($pos = strpos($code, $l)) !== FALSE && ($pos + strlen($l)) === strlen($code)) {
        $lang = $code;
        break;
      }
    }

    if ($lang === NULL) {
      $lang = $default;
    }
  }

  return $lang;
}

/**
 * Converts a DocCheck language name to a Drupal language name.
 *
 * @param $lang
 *   DocCheck language name.
 *
 * @return bool|string
 *   The converted language name or FALSE, if the conversion is unsuccessful.
 */
function doccheck_get_drupal_language($lang) {
  $doccheck_languages = doccheck_get_languages();

  if (array_key_exists($lang, $doccheck_languages)) {
    return $lang == 'com' ? 'en' : $lang;
  }

  return FALSE;
}
