<?php
/**
 * @file
 * Commerce SoColissimo Point
 *
 * Available $element attributes :
 * - #icon: icon url
 * - #ease_of_access: an array of ease of access icons url.
 *   Loop over and display associated icon
 * - #adresse: adresse string
 * - #opening_hours: an array of hours.
 *   Title key is day label, value key is hour
 * - #codePostal: code postal string
 * - #congesPartiel: boolean indicating the merchant is closed
 *   for a short perdiod. see #listConges
 * - #listeConges: object containing calendarDeDebut and calendarDeFin
 *   date value
 * - #congesTotal: boolean indicating the merchant is closed.
 *   Point should be disabled
 * - #coordGeolocalisationLatitude: latitude
 * - #coordGeolocalisationLongitude: longitude
 * - #distanceEnMetre: distince in meters from #adresse
 * - #identifiant: point id
 * - #indiceDeLocalisation: additional helper informations about location
 * - #localite: city
 * - #nom: point title
 * - #periodeActiviteHoraireDeb: effective date for #opening_hours
 * - #periodeActiviteHoraireFin: end period of effective date for #opening_hours
 * - #poidsMaxi: maximum parcel weight
 * - #typeDePoint: point type
 * - #codePays: iso country code
 * - #langue: language code
 * - #libellePays: country full name
 * - #reseau: technical network id which point belong to
 * - #distributionSort: specific delivery informations for customer
 * - #lotAcheminement: specific delivery informations for customer
 * - #versionPlanTri: specific delivery informations for customer
 */
?>
<div
  class="selected-point-infos"><?php print t('Your selected point:'); ?></div>
<div class="point_informations">
  <div class="name"><?php print $element['#nom']; ?></div>
  <div class="address"><?php print $element['#adresse']; ?></div>
  <div class="postalcode_city">
    <span class="postalcode"><?php print $element['#codePostal']; ?></span>
    <span class="city"><?php print $element['#localite']; ?></span>
  </div>
  <?php if ($element['#ease_of_access']) : ?>
    <ul class="ease-of-access">
      <?php foreach ($element['#ease_of_access'] as $icon_src): ?>
        <li><img src="<?php print $icon_src; ?>" alt="" width="16" height="16"/>
        </li>
      <?php endforeach; ?>
    </ul>
  <?php endif; ?>
</div>
<div class="clear"></div>
