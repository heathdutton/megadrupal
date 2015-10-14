(function ($) {
	var unblock;
	$(document).ajaxSend(function (event, jqxhr, settings) {
		if (!unblock) {
			var target = ajaxScreenLock.getUrlPath(settings.url);
			var pages = Drupal.settings.ajaxScreenLock.pages;
			var visibility = Drupal.settings.ajaxScreenLock.visibility;
			var disabled = Drupal.settings.ajaxScreenLock.disabled;

			// If is not disabled.
			if (!disabled) {
				if (!$.isEmptyObject(pages)) {
					// Handle pages.
					$.each(pages, function (num, page) {
						page = ajaxScreenLock.getUrlPath(page);
						if (target.length >= page.trim().length) {
							if (target.substr(0, page.trim().length) == page.trim() && visibility == 1) {
								ajaxScreenLock.blockUI();
							}
							else if (visibility == 0 && target.substr(0, page.trim().length) != page.trim()) {
								ajaxScreenLock.blockUI();
							}
						}
					});
				}
				else {
					ajaxScreenLock.blockUI();
				}
			}
		}
	});

	$(document).ajaxStop(function (r, s) {
		if (unblock) {
			$.unblockUI();
			unblock = false;
		}
	});


	var ajaxScreenLock = {
		// Grab path from AJAX url.
		getUrlPath: function (ajaxUrl) {
			var url = document.createElement("a");
			url.href = ajaxUrl;
			return url.pathname;
		},

		blockUI: function () {
			unblock = true;
			if (Drupal.settings.ajaxScreenLock.throbber_hide) {
				$('.ajax-progress-throbber').hide();
			}

			$.blockUI({
				message: Drupal.settings.ajaxScreenLock.message,
				css: {
					top: ($(window).height() - 400) / 2 + 'px',
					left: ($(window).width() - 400) / 2 + 'px',
					width: '400px'
				},
				timeout: Drupal.settings.ajaxScreenLock.timeout
			});
		}
	}
}(jQuery));