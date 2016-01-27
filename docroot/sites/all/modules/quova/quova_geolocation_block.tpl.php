<ul>
  <li><?php echo t('IP') . ': ' . $info->ipinfo->ip_address; ?></li>
  <li><?php echo t('Continent') . ': ' . $info->ipinfo->Location->continent; ?></li>
  <li><?php echo t('Country') . ': ' . $info->ipinfo->Location->CountryData->country; ?></li>
  <li><?php echo t('State') . ': ' . $info->ipinfo->Location->StateData->state; ?></li>
  <li><?php echo t('City') . ': ' . $info->ipinfo->Location->CityData->city; ?></li>
</ul>
