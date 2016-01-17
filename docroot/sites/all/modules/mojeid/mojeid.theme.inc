<?php

function theme_mojeid_button_links() {
	return '<div class="mojeid-button-links">'. l(t('What is mojeID?'), MOJEID_URL_ABOUT) .' | '. l(t('Create new mojeID account'), MOJEID_URL_CREATE_ACCOUNT) .'</div>';
}
