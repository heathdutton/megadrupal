<?php

// Campaign Info.
echo l(t('Select another campaign'), 'salsa/campaigns');
echo '<p>' . $my_donate_page->Description . '</p>';

// Header.
echo '<h2>' . t('Your fundraising pages') . '</h2>';
echo '<p>' . l(t('Create a new page'), 'salsa/my_donate_page/' . $my_donate_page->my_donate_page_KEY . '/create') . '</p>';

// Pages.
if (!empty($pages)) {
  foreach ($pages as $page) {
    echo '<div class="salsa-supporter-my-donate-page">';
    echo '<h3>' . l($page->Page_Title, 'salsa/supporter_my_donate_page/' . $page->supporter_my_donate_page_KEY) . '</h3>';
    echo '<div class="salsa-my-donate-page-info">';
    echo l(t('Modify your page'), 'salsa/my_donate_page/' . $my_donate_page->my_donate_page_KEY . '/edit/' . $page->supporter_my_donate_page_KEY) . '<br />';
    echo t('Fundraising Page Goal: @amount', array('@amount' => $page->Donation_Goal)) . '<br />';

    if (isset($money_raised[$page->supporter_my_donate_page_KEY])) {
      echo t('Money raised: @amount', array('@amount' => $money_raised[$page->supporter_my_donate_page_KEY]));
    }
    else {
      echo t('No donations have been made on this page.') . '<br />';
    }

    if (!empty($donors[$page->supporter_my_donate_page_KEY])) {
      echo '<h4>' . t('Donors') . '</h4>';
      echo '<ul>';
      foreach ($donors[$page->supporter_my_donate_page_KEY] as $donor) {
        $donor_name = $donor['Hide_Last_Name'] ? $donor['First_Name'] : $donor['First_Name'] . ' ' . $donor['Last_Name'];
        echo '<li>' . $donor_name . ' (' . $donor['Email'] . '): ' . $donor['Currency_Code'] . ' ' . $donor['Amount'] . '</li>';
      }
      echo '</ul>';
    }
    echo '</div>';
    echo '</div>';
  }
}
