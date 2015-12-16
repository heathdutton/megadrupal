/**
 * @file media_tudou/js/media_tudou.js
 */

(function($) {

	Drupal.media_tudou = {};
	Drupal.behaviors.media_tudou = {
		attach : function(context, settings) {
			// Check the browser to see if it supports html5 video.
			var video = document.createElement('video');
			var html5 = video.canPlayType ? true : false;

			// If it has video, does it support the correct codecs?
			if (html5) {
				html5 = false;
				if (video.canPlayType('video/webm; codecs="vp8, vorbis"')
						|| video
								.canPlayType('video/mp4; codecs="avc1.42E01E, mp4a.40.2"')) {
					html5 = true;
				}
			}

			// Put a prompt in the video wrappers to let users know they need
			// flash
			if (!FlashDetect.installed && !html5) {
				$('.media-tudou-preview-wrapper').each(
						Drupal.media_tudou.needFlash);
			}
		}
	};

	Drupal.media_tudou.needFlash = function() {
		var id = $(this).attr('id');
		var wrapper = $('.media-tudou-preview-wrapper');
		var hw = Drupal.settings.media_tudou[id].height
				/ Drupal.settings.media_tudou[id].width;
		wrapper
				.html('<div class="js-fallback">'
						+ Drupal
								.t(
										'You need Flash to watch this video. <a href="@flash">Get Flash</a>',
										{
											'@flash' : 'http://get.adobe.com/flashplayer'
										}) + '</div>');
		wrapper.height(wrapper.width() * hw);
	};

	Drupal.media_tudou.insertEmbed = function(embed_id) {
		var videoWrapper = $('#' + embed_id + '.media-tudou-preview-wrapper');
		var settings = Drupal.settings.media_tudou[embed_id];

		// Calculate the ratio of the dimensions of the embed.
		settings.hw = settings.height / settings.width;

		// Replace the object embed with TuDou's iframe. This isn't done by
		// the
		// theme function because TuDou doesn't have a no-JS or no-Flash
		// fallback.
		var video = $('<iframe class="tudou-player" type="text/html" frameborder="0"></iframe>');
		var src = 'http://www.tudou.com/embed/' + settings.video_id;

		// Allow other modules to modify the video settings.
		settings.options.wmode = 'opaque';
		$(window).trigger('media_tudou_load', settings);

		// Merge TuDou options (such as autoplay) into the source URL.
		var query = $.param(settings.options);
		if (query) {
			src += '?' + query;
		}

		// Set up the iframe with its contents and add it to the page.
		video.attr('id', settings.id).attr('width', settings.width).attr(
				'height', settings.height).attr('src', src);
		videoWrapper.html(video);

		// Bind a resize event to handle fluid layouts.
		$(window).bind('resize', Drupal.media_tudou.resizeEmbeds);

		// For some reason Chrome does not properly size the container around
		// the
		// embed and it will just render the embed at full size unless we set
		// this
		// timeout.
		if (!$('.lightbox-stack').length) {
			setTimeout(Drupal.media_tudou.resizeEmbeds, 1);
		}
	};

	Drupal.media_tudou.resizeEmbeds = function() {
		$('.media-tudou-preview-wrapper').each(
				Drupal.media_tudou.resizeEmbed);
	};

	Drupal.media_tudou.resizeEmbed = function() {
		var context = $(this).parent();
		var video = $(this).children(':first-child');
		var hw = Drupal.settings.media_tudou[$(this).attr('id')].hw;
		// Change the height of the wrapper that was given a fixed height by the
		// TuDou theming function.
		$(this).height(context.width() * hw).width(context.width());

		// Change the attributes on the embed to match the new size.
		video.height(context.width() * hw).width(context.width());
	};

})(jQuery);