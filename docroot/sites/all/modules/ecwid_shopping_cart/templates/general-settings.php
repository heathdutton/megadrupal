<div class="wrap pure-form ecwid-settings general-settings">

  <h2><?php echo t('Ecwid Shopping Cart — General settings'); ?></h2>
  <fieldset>

    <input type="hidden" name="settings_section" value="general" />

    <div class="greeting-box complete">
      <div class="image-container">
        <img class="greeting-image" src="<?php echo file_create_url(drupal_get_path('module', 'ecwid_shopping_cart'), array('absolute' => true)); ?>/images/store_ready.png" width="142" />
      </div>

      <div class="messages-container">
        <?php if (array_key_exists('settings-updated', $_GET)): ?>

        <div class="main-message"><?php echo t('Congratulations!'); ?></div>
        <div class="secondary-message"?><?php echo t('Your Ecwid store is now connected to your Drupal website'); ?></div>

        <?php else: ?>

        <div class="main-message"><?php echo t('Greetings!'); ?></div>
        <div class="secondary-message"?><?php echo t('Your Ecwid store is connected to your Drupal website'); ?></div>
        <?php endif; ?>
      </div>
    </div>
    <hr />
    <div class="section">
      <div class="left">
        <span class="main-info">
          <?php echo t('Store ID'); ?>: <strong><?php echo check_plain(_ecwid_shopping_cart_get_storeid()); ?></strong>
        </span>
      </div>
      <div class="right two-buttons">
        <a class="pure-button" target="_blank" href="https://my.ecwid.com/cp/?source=drupal7#t1=&t2=Dashboard">
          <?php echo t('Control panel'); ?>
        </a>
        <a class="pure-button" target="_blank" href="<?php echo check_plain(url('store')); ?>">
          <?php echo t('Visit storefront'); ?>
        </a>
      </div>
    </div>

    <?php if (_ecwid_shopping_cart_get_storeid() == 1003 || !ecwid_is_api_enabled(_ecwid_shopping_cart_get_storeid())): ?>
    <div class="section account-section">
      <div class="left">
        <div class="secondary-info">
          <?php echo t('Upgrade your account to get access to more Ecwid plugin features'); ?>
        </div>
      </div>

      <div class="right">
        <a target="_blank" class="pure-button pure-button-primary" href="http://www.ecwid.com/plans-and-pricing.html?source=drupal7">
          <?php echo t('Upgrade'); ?>
        </a>
      </div>
    </div>
    <?php endif; ?>

    <div class="note grayed-links">
      <?php
        echo sprintf(
          t('If you want to connect another Ecwid store, you can <a %s>disconnect the current one and change Store ID</a>.'),
          'href="#" onClick="javascript:document.getElementById(\'ecwid-shopping-cart-general-page\').submit(); return false;"'
        );
      ?>

    </div>

    <hr />
    <p><?php echo t('Questions? Visit <a href="http://help.ecwid.com/?source=drupal7">Ecwid support center</a>'); ?></p>
  </fieldset>
</div>
