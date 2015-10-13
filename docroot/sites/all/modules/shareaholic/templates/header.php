<?php ShareaholicAdmin::include_css_js_assets(); ?>
<script>
  window.first_part_of_url = "<?php echo $settings['base_link']; ?>";
  window.verification_key = "<?php echo $settings['verification_key']; ?>";
  window.shareaholic_api_key = "<?php echo $settings['api_key']; ?>";
  window.SHAREAHOLIC_PLUGIN_VERSION = '<?php echo ShareaholicUtilities::get_version(); ?>';
</script>

<script>
  // override default jquery with sQuery (from jquery_custom) on shareaholic admin pages only
  $ = jQuery = sQuery;
</script>

<script type="text/javascript"
        data-sorendpoint="<?php echo ShareaholicUtilities::URL ?>"
        data-sorassetbase="<?php echo rtrim(ShareaholicUtilities::asset_url(''), '/') ?>"
        src="<?php echo ShareaholicUtilities::asset_url('headerWidget.js') ?>">
</script>
<link href="<?php echo ShareaholicUtilities::asset_url('header.css'); ?>" media="all" rel="stylesheet" type="text/css">


<!-- Start Header Template -->
<div class="new-design flat-layout drupal-admin" style="display: none;">
  <header class="header-section js-header">
    <div class="header">
      <a href= "https://shareaholic.com" class="logo"></a>
      <nav class="header-nav">
      </nav>
      <ul class="user-nav js-logged-out" style="display:none;">
        <li><a href="#" class="btn dark-green js-action-signup">Sign Up Free</a></li>
        <li><a href="#" class="btn js-action-login">Log In</a></li>
      </ul>
      <!-- Logged In -->
      <div class="user-nav logged-in js-logged-in" style="display:none;">

        <a class="btn user-menu-btn" href="javascript:void">
          <span class="mask"></span>
          <span class="js-user username">Jones</span> <span class="fa fa-angle-down"></span></a>
        <ul class="user-menu">
          <li class="warning divider js-error-twitter" style="display:none;"><a href="#" class="js-action-twitterauth"><i class="caution-icon"></i> Reconnect Twitter Account</a></li>
          <li><a href="<?php echo $settings['website_settings_link']; ?>" class="dashboard" target="_blank">Website Settings</a></li>
          <li><a href="#" class="js-action-admanager divider" target="_blank">Ad Manager</a></li>
          <li><a href="#" class="js-action-profile" target="_blank">My Profile</a></li>
          <li><a href="#" class="js-action-settings" target="_blank">Account</a></li>
          <li><a href="#" class="js-action-logout divider">Log Out</a></li>
          <li><a href="#" class="help js-action-help" target="_blank">Help</a></li>
        </ul>
        <img class="avatar js-avatar">
        <div class="hiring">
          <a href="https://shareaholic.com/careers?utm_source=univ_header_drupal_plugin&utm_medium=link&utm_campaign=careers" target="_blank">We're Hiring</a>
        </div>
      </div>
      <!-- Mobile Start -->
      <a href="#" class="mobile-menu-link"><span class="fa fa-bars"></span></a>
    </div>
    <!-- Mobile Menu-->
    <div class="mobile-menu">
      <!-- Logged Out-->
      <ul class="js-logged-out" style="display:none;">
         <div class="hiring">
          <a href="https://shareaholic.com/careers?utm_source=univ_header_drupal_plugin&utm_medium=link&utm_campaign=careers" target="_blank">We're Hiring</a>
        </div>
        <li><a href="/publishers">For <strong>Publishers</strong></a></li>
        <li><a href="/advertisers">For <strong>Advertisers</strong></a></li>
        <li><a href="#" class="js-action-login">Log In</a></li>
        <li><a href="#" class="js-action-signup">Sign Up for FREE</a></li>
      </ul>
      <!-- Logged In-->
      <ul class="js-logged-in" style="display:none;">
        <li class="warning js-error-twitter" style="display:none;"><a href="#" class="js-action-twitterauth "><i class="caution-icon"></i> Reconnect Twitter Account</a></li>
        <li><a href="#" class="dashboard js-action-dashboard">Website Dashboard</a></li>
        <li><a href="#" class="js-action-admanager">Ad Manager</a></li>
        <li><a href="#" class="js-action-profile">My Profile</a></li>
        <li><a href="#" class="js-action-settings">Account</a></li>
        <li><a href="#" class="js-action-logout">Log Out</a></li>
        <li><a href="#" class="help js-action-help">Help</a></li>
      </ul>
    </div>
  </header>
</div>
<!-- End Header Template -->


<script>

  (function($) {

    function formatUniversalHeader() {
      var $header = $('.new-design');
      var $target = $('.region.region-content');

      // move the header to the target and show it to the user
      $target.before($header);
      $header.show();
    }


    $(function() {
      formatUniversalHeader();
    });

  })(jQuery);

</script>
