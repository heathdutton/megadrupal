<div class="wrap pure-form ecwid-settings general-settings">

		<h2><?php echo t('Ecwid Shopping Cart — General settings'); ?></h2>
		<fieldset>

			<input type="hidden" name="settings_section" value="general" />

			<div class="greeting-box">

				<div class="image-container">
					<img class="greeting-image" src="<?php echo file_create_url(drupal_get_path('module', 'ecwid_shopping_cart'), array('absolute' => true)); ?>/images/store_inprogress.png" width="142" />
				</div>

				<div class="messages-container">
					<div class="main-message">

						<?php echo t('Thank you for choosing Ecwid to build your online store'); ?>
					</div>
					<div class="secondary-message">
						<?php echo t('The first step towards opening your online business: <br />Let’s get started and add a store to your Drupal website in <strong>3</strong> simple steps.'); ?>
					</div>
				</div>

			</div>
			<hr />

			<ol>
				<li>
					<h4><?php echo t('Register at Ecwid'); ?></h4>
					<div>
						<?php echo t('Create a new Ecwid account which you will use to manage your store and inventory. The registration is free.'); ?>
					</div>
					<div class="ecwid-account-buttons">
						<a class="pure-button pure-button-secondary" target="_blank" href="<?php echo _ecwid_shopping_cart_get_register_link(); ?>">
							<?php echo t('Create new Ecwid account'); ?>
						</a>
						<a class="pure-button pure-button-secondary" target="_blank" href="https://my.ecwid.com/cp/?source=drupal7#t1=&t2=Dashboard">
							<?php echo t('I already have Ecwid account, sign in'); ?>
						</a>
					</div>
					<div class="note">
						<?php echo t('You will be able to sign up through your existing Google, Facebook or PayPal profiles as well.'); ?>
					</div>
				</li>
				<li>
					<h4><?php echo t('Find your Store ID'); ?></h4>
					<div>
						<?php echo t('Store ID is a unique identifier of any Ecwid store, it consists of several digits. You can find it on the "Dashboard" page of Ecwid control panel. Also the Store ID will be sent in the Welcome email after the registration.'); ?>
					</div>
				</li>
				<li>
					<h4>
						<?php echo t('Enter your Store ID'); ?>
					</h4>
					<div><label for="ecwid_store_id"><?php echo t('Enter your Store ID here:'); ?></label></div>
					<div class="pure-control-group store-id">
						<input
							id="ecwid_store_id"
							name="ecwid_store_id"
							type="text"
							placeholder="<?php echo t('Store ID'); ?>"
							value="<?php if (_ecwid_shopping_cart_get_storeid() != 1003) echo check_plain(_ecwid_shopping_cart_get_storeid()); ?>"
							/>
						<button type="submit" class="pure-button pure-button-primary" name="op" value="Save configuration'"><?php echo t('Save and connect your Ecwid store to the site'); ?></button>
					</div>

				</li>
			</ol>
			<hr />
			<p><?php echo t('Questions? Visit <a href="http://help.ecwid.com/?source=drupal7">Ecwid support center</a>'); ?></p>
		</fieldset>
</div>
