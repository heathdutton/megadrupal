<?php
/**
 * @file
 * Commerce So Colissimo Point details
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
 * - #distanceEnMetre: distance in meters from #adresse
 * - #identifiant: point class
 * - #indiceDeLocalisation: additional helper information about location
 * - #localite: city
 * - #nom: point title
 * - #periodeActiviteHoraireDeb: effective date for #opening_hours
 * - #periodeActiviteHoraireFin: end period of effective date for #opening_hours
 * - #poidsMaxi: maximum parcel weight
 * - #typeDePoint: point type
 * - #codePays: iso country code
 * - #langue: language code
 * - #libellePays: country full name
 * - #reseau: technical network class which point belong to
 * - #distributionSort: specific delivery informations for customer
 * - #lotAcheminement: specific delivery informations for customer
 * - #versionPlanTri: specific delivery informations for customer
 */
?>
<div class="<?php print $classes; ?>"<?php print $attributes; ?>>
  <?php if (isset($element['#title'])): ?>
    <div class="field-label"><?php print $element['#title']; ?></div>
  <?php endif; ?>
  <div class="map_details">
    <?php if (isset($element['#phone_number'])): ?>
      <div class="customer_phone_number">
        <div class="phone_number">
          <span><?php print t('Contact phone: !phone_number', array('!phone_number' => $element['#phone_number'])) ?></span>
        </div>
      </div>
    <?php endif; ?>
    <?php if (isset($element['#delivery_delay'])): ?>
      <div class="delivery_delay">
        <div class="delivery_delay_label">
          <span><?php print t("Shipping under !day days", array('!day' => $element['#delivery_delay'])); ?></span>
        </div>
      </div>
    <?php endif; ?>
    <div class="point_informations_details">
      <div class="name"><?php print $element['#nom']; ?></div>
      <div class="address"><?php print $element['#adresse']; ?></div>
      <div class="postalcode_city">
        <span class="postalcode"><?php print $element['#codePostal']; ?></span>
        <span class="city"><?php print $element['#localite']; ?></span>
      </div>
      <?php if ($element['#ease_of_access']) : ?>
        <ul class="ease-of-access">
          <?php foreach ($element['#ease_of_access'] as $icon_src): ?>
            <li><img src="<?php print $icon_src; ?>" alt="" wclassth="16"
                     height="16"/></li>
          <?php endforeach; ?>
        </ul>
      <?php endif; ?>
    </div>
    <div class="hours">
      <span><?php print t('Opening and closing hours :'); ?></span>
      <table>
        <?php foreach ($element['#opening_hours'] as $key => $hour): ?>
          <tr class="<?php print ($key % 2) ? 'even' : 'odd'; ?>">
            <td class="day">
              <?php print $hour['title']; ?>
            </td>
            <td class="open_hours">
              <?php print $hour['value']; ?>
            </td>
          </tr>
        <?php endforeach; ?>
      </table>
    </div>
  </div>
  <div class="map_tip"></div>
</div>
