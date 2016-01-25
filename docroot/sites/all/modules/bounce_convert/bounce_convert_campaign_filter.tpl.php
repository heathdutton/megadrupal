<?php
/**
 * @file
 * Campaign search form.
 *
 *
 * Available variables:
 * - $base_url: Base url of the site
 */
?>

<div class="bounce-convert-campaign-filter-form">
  <br />
  <form action="<?php print $path . '/admin/reports/bounce-convert'; ?>" method="post">
    <input type="text" id="campaign" name="campaign" placeholder="Campaign name" />
    <input type="submit" name="search" value="Search" />
  </form>
</div>
<br />
