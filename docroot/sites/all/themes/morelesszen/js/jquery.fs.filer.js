/*
 * Filer Plugin [for use with Formstone Library]
 */
 
if (jQuery) (function($) {
	
	// Mobile Detect
	var agent = isMobile = /Android|webOS|iPhone|iPad|iPod|BlackBerry/i.test( (navigator.userAgent||navigator.vendor||window.opera) );
	
	// Default Options
	var options = {
		customClass: ""
	};
	
	// Public Methods
	var pub = {
		
		// Set Defaults
		defaults: function(opts) {
			options = $.extend(options, opts || {});
			return $(this);
		},
		
		// Disable field
		disable: function() {
			return $(this).each(function(i) {
				var $input = $(this),
					$filer = $input.parent(".filer");
				
				$input.attr("disabled", "disabled");
				$filer.addClass("disabled");
			});
		},
		
		// Enable field
		enable: function() {
			return $(this).each(function(i) {
				var $input = $(this),
					$filer = $input.parent(".filer");
				
				$input.attr("disabled", null);
				$filer.removeClass("disabled");
			});
		},
		
		// Destroy filer
		destroy: function() {
			return $(this).each(function(i) {
				var $input = $(this),
					$filer = $input.parent(".filer");
				
				// Unbind click events
				$filer.off(".filer")
						.find(".stepper-step")
						.remove();
				
				// Restore DOM
				$input.unwrap()
					  .removeClass("filer-input");
			});
		}
	};
	
	// Private Methods
	
	// Initialize
	function _init(opts) {
		opts = opts || {};
		// Check for mobile
		if (isMobile) {
			opts.trueMobile = true;
			if (typeof opts.mobile === "undefined") {
				opts.mobile = true;
			}
		}
		
		// Define settings
		var settings = $.extend({}, options, opts);
		
		// Apply to each element
		return $(this).each(function(i) {
			var $input = $(this);
			
			if (!$input.data("filer")) {
				var min = parseFloat($input.attr("min")),
					max = parseFloat($input.attr("max")),
					step = parseFloat($input.attr("step")) || 1;
				
				// Modify DOM
				$input.addClass("filer-input")
					  .wrap('<div class="filer ' + settings.customClass + '" />')
					  .after('<div class="no-file-selected filer-filename"></div><div class="filer-browse">Browse</div>');
				
				// Store plugin data
				var $filer = $input.parent(".filer");
				
				// Check disabled
				if ($input.is(":disabled")) {
					$filer.addClass("disabled");
				}
				
				var data = $.extend({
					$filer: $filer,
					$input: $input,
					$filename: $filer.find(".filer-filename"),
					$browse: $filer.find(".filer-browse")
				}, settings);
				
				// Bind click events
				$filer.on("click.filer", ".filer-browse", data, _browse)
						.data("filer", data);
				$input.on("change.filer", data, _update);
			_updateFilename(data);
			}
		});
	}
	
	// Handle click
	function _browse(e) {
		e.preventDefault();
		e.stopPropagation();
		
		var data = e.data;
		
		if (!data.$filer.hasClass("disabled")) {
			// open the open popup by triggering a click on the original element
			data.$input.click();
		}
	}
	
	// Handle update
	function _update(e) {
		e.preventDefault();
		e.stopPropagation();
		
		var data = e.data;
		
		if (!data.$filer.hasClass("disabled")) {
			_updateFilename(data);
		}
	}
	
	function _updateFilename(data) {
		var files = data.$input[0].value;
		if (files) {
			data.$filename.html(files);
			data.$filename.removeClass('no-file-selected').addClass('file-selected');
		} else {
			data.$filename.html('no files');
			data.$filename.removeClass('file-selected').addClass('no-file-selected');
		}
	}
	
	
	// Define Plugin
	$.fn.filer = function(method) {
		if (pub[method]) {
			return pub[method].apply(this, Array.prototype.slice.call(arguments, 1));
		} else if (typeof method === 'object' || !method) {
			return _init.apply(this, arguments);
		}
		return this;
	};
})(jQuery);
