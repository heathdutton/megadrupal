<?php

/*
 * @file
 * This implementation of the Paice Husk Stemmer is free software.
 * It has originally been written by Alexis Ulrich (http://alx2002.free.fr)
 * It was modified by Frederic Bisson for Drupal integration.
 */

/**
 * Returns basic replacement patterns to be used by the stemmer.
 */
function _frenchstemmer_replacements() {

  $pattern=array();
  $replace=array();

  $pattern[]='/bb/';
  $replace[]='b';
  $pattern[]='/dd/';
  $replace[]='d';
  $pattern[]='/ff/';
  $replace[]='f';
  $pattern[]='/gg/';
  $replace[]='g';
  $pattern[]='/ll/';
  $replace[]='l';
  $pattern[]='/mm/';
  $replace[]='m';
  $pattern[]='/nn/';
  $replace[]='n';
  $pattern[]='/pp/';
  $replace[]='p';
  $pattern[]='/rr/';
  $replace[]='r';
  $pattern[]='/tt/';
  $replace[]='t';
  $pattern[]='/vv/';
  $replace[]='v';
  $pattern[]='/xx/';
  $replace[]='x';
  $pattern[]='/zz/';
  $replace[]='z';
  $pattern[]='/ph/';
  $replace[]='f';
  $pattern[]='/qu/';
  $replace[]='k';

  $pattern[]='/[cC]([kaouKAOUâàáôöùûüÂÀÔÖÙÛÜ])/u';
  $replace[]='k\\1';
  $pattern[]='/([aeiouAEIOUâàáéèêëîïôöùûüÂÀÉÈÊËÎÏÔÖÙÛÜ])[sS]([aeiouAEIOUâàáéèêëîïôöùûüÂÀÉÈÊËÎÏÔÖÙÛÜ])/u';
  $replace[]='\\1z\\2';
  $pattern[]='/ss/';
  $replace[]='s';
  $pattern[]='/([sS]?)[cC]([eiEIéèêëîïÉÈÊËÎÏ])/u';
  $replace[]='\\1s\\2';
  $pattern[]='/c[^h]/';
  $replace[]='k';
  $pattern[]='/[kK][sS]/';
  $replace[]='x';
  $pattern[]='/[yY][bcdfghjklmnpqrstvwxzBCDFGHJKLMNPQRSTVWXZ]/';
  $replace[]='i';

  $pattern[]='/eau/';
  $replace[]='o';
  $pattern[]='/au/';
  $replace[]='o';

  return array('pattern' => $pattern, 'replace' => $replace);
}


/**
 * Returns french stemmer rules variables.
 */
function _frenchstemmer_rules_vars() {
  $vars = array();

  // french accented letters in ISO-8859-1 encoding
  $vars['accents'] = chr(224).chr(226).chr(232).chr(233).chr(234).chr(235).chr(238).chr(239).chr(244).chr(251).chr(249).chr(231);

  // the rule patterns include all accented forms for french language
  $vars['rule_pattern'] = "/^([a-z" . $vars['accents'] . "]*)(\*){0,1}(\d)([a-z" . $vars['accents'] . "]*)([.|>])/";

  // french vowels (including y) in ISO-8859-1 encoding
  $vars['vowels'] = chr(97).chr(224).chr(226).chr(101).chr(232).chr(233).chr(234).chr(235).chr(105).chr(238).chr(239).chr(111).chr(244).chr(117).chr(251).chr(249).chr(121);

// the french rules in ISO-8859-1 encoding
$vars['rules'] = array(
  'esre1>','esio1>','siol1.','siof0.','sioe0.','sio3>','st1>','sf1>','sle1>',
  'slo1>','s'.chr(233).'1>',chr(233).'tuae5.',chr(233).'tuae2.','tnia0.','tniv1.','tni3>','suor1.',
  'suo0.','sdrail5.','sdrai4.','er'.chr(232).'i1>','sesue3x>','esuey5i.','esue2x>',
  'se1>','er'.chr(232).'g3.','eca1>','esiah0.','esi1>','siss2.','sir2>','sit2>','egan'.chr(233).'1.',
  'egalli6>','egass1.','egas0.','egat3.','ega3>','ette4>','ett2>','etio1.',
  'tio'.chr(231).'4c.','tio0.','et1>','eb1>','snia1>','eniatnau8>','eniatn4.','enia1>',
  'niatnio3.','niatg3.','e'.chr(233).'1>',''.chr(233).'hcat1.',''.chr(233).'hca4.',''.chr(233).'tila5>',''.chr(233).'tici5.',''.chr(233).'tir1.',
  ''.chr(233).'ti3>',''.chr(233).'gan1.',''.chr(233).'ga3>',''.chr(233).'tehc1.',''.chr(233).'te3>',''.chr(233).'it0.',''.chr(233).'1>','eire4.','eirue5.',
  'eio1.','eia1.','ei1>','eng1.','xuaessi7.','xuae1>','uaes0.','uae3.',
  'xuave2l.','xuav2li>','xua3la>','ela1>','lart2.','lani2>','la'.chr(233).'2>','siay4i.',
  'siassia7.','siarv1*.','sia1>','tneiayo6i.','tneiay6i.','tneiassia9.',
  'tneiareio7.','tneia5>','tneia4>','tiario4.','tiarim3.','tiaria3.',
  'tiaris3.','tiari5.','tiarve6>','tiare5>','iare4>','are3>','tiay4i.',
  'tia3>','tnay4i.','em'.chr(232).'iu5>','em'.chr(232).'i4>','tnaun3.','tnauqo3.','tnau4>','tnaf0.',
  'tnat'.chr(233).'2>','tna3>','tno3>','zeiy4i.','zey3i.','zeire5>','zeird4.','zeirio4.',
  'ze2>','ssiab0.','ssia4.','ssi3.','tnemma6>','tnemesuey9i.','tnemesue8>',
  'tnemevi7.','tnemessia5.','tnemessi8.','tneme5>','tnemia4.','tnem'.chr(233).'5>',
  'el2l>','lle3le>','let'.chr(244).'0.','lepp0.','le2>','srei1>','reit3.','reila2.',
  'rei3>','ert'.chr(226).'e5.','ert'.chr(226).''.chr(233).'1.','ert'.chr(226).'4.','drai4.','erdro0.','erute5.','ruta0.',
  'eruta1.','erutiov1.','erub3.','eruh3.','erul3.','er2r>','nn1>','r'.chr(232).'i3.',
  'srev0.','sr1>','rid2>','re2>','xuei4.','esuei5.','lbati3.','lba3>',
  'rueis0.','ruehcn4.','ecirta6.','ruetai6.','rueta5.','rueir0.','rue3>',
  'esseti6.','essere6>','esserd1.','esse4>','essiab1.','essia5.','essio1.',
  'essi4.','essal4.','essa1>','ssab1.','essurp1.','essu4.','essi1.','ssor1.',
  'essor2.','esso1>','ess2>','tio3.','r'.chr(232).'s2re.','r'.chr(232).'0e.','esn1.','eu1>',
  'sua0.','su1>','utt1>','tu'.chr(231).'3c.','u'.chr(231).'2c.','ur1.','ehcn2>','ehcu1>','snorr3.',
  'snoru3.','snorua3.','snorv3.','snorio4.','snori5.','snore5>','snortt4>',
  'snort'.chr(238).'a7.','snort3.','snor4.','snossi6.','snoire6.','snoird5.','snoitai7.',
  'snoita6.','snoits1>','noits0.','snoi4>','noitaci7>','noitai6.','noita5.',
  'noitu4.','noi3>','snoya0.','snoy4i.','sno'.chr(231).'a1.','sno'.chr(231).'r1.','snoe4.',
  'snosiar1>','snola1.','sno3>','sno1>','noll2.','tnennei4.','ennei2>',
  'snei1>','sne'.chr(233).'1>','enne'.chr(233).'5e.','ne'.chr(233).'3e.','neic0.','neiv0.','nei3.','sc1.',
  'sd1.','sg1.','sni1.','tiu0.','ti2.','sp1>','sna1>','sue1.','enn2>','nong2.',
  'noss2.','rioe4.','riot0.','riorc1.','riovec5.','rio3.','ric2.','ril2.',
  'tnerim3.','tneris3>','tneri5.','t'.chr(238).'a3.','riss2.','t'.chr(238).'2.','t'.chr(226).'2>','ario2.',
  'arim1.','ara1.','aris1.','ari3.','art1>','ardn2.','arr1.','arua1.','aro1.',
  'arv1.','aru1.','ar2.','rd1.','ud1.','ul1.','ini1.','rin2.','tnessiab3.',
  'tnessia7.','tnessi6.','tnessni4.','sini2.','sl1.','iard3.','iario3.','ia2>',
  'io0.','iule2.','i1>','sid2.','sic2.','esoi4.','ed1.','ai2>','a1>','adr1.',
  'tner'.chr(232).'5>','evir1.','evio4>','evi3.','fita4.','fi2>','enie1.','sare4>',
  'sari4>','sard3.','sart2>','sa2.','tnessa6>','tnessu6>','tnegna3.','tnegi3.',
  'tneg0.','tneru5>','tnemg0.','tnerni4.','tneiv1.','tne3>','une1.','en1>',
  'nitn2.','ecnay5i.','ecnal1.','ecna4.','ec1>','nn1.','rit2>','rut2>','rud2.',
  'ugn1>','eg1>','tuo0.','tul2>','t'.chr(251).'2>','ev1>','v'.chr(232).'2ve>','rtt1>','emissi6.',
  'em1.','ehc1.','c'.chr(233).'i2c'.chr(232).'.','libi2l.','llie1.','liei4i.','xuev1.','xuey4i.',
  'xueni5>','xuell4.','xuere5.','xue3>','rb'.chr(233).'3rb'.chr(232).'.','tur2.','rir'.chr(233).'4re.','rir2.',
  'c'.chr(226).'2ca.','snu1.','rt'.chr(238).'a4.','long2.','vec2.',''.chr(231).'1c>','ssilp3.','silp2.',
  't'.chr(232).'hc2te.','n'.chr(232).'m2ne.','llepp1.','tan2.','rv'.chr(232).'3rve.','rv'.chr(233).'3rve.','r'.chr(232).'2re.',
  'r'.chr(233).'2re.','t'.chr(232).'2te.','t'.chr(233).'2te.','epp1.','eya2i.','ya1i.','yo1i.','esu1.','ugi1.',
  'tt1.',
  
  'end0.'
);

  return $vars;
}
