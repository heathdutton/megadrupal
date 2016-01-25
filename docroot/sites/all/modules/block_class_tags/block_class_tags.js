/**
* @file
* block_class_tags.js
*
* Initialize taggle.
*/

var Drupal = Drupal || {};

(function($, Drupal){
	"use strict";

	Drupal.behaviors.block_class_tags = {
		attach: function(context, settings) {

			// Get current tags and enabled plugins.
			var tags = (settings.block_class_tags_tags) ? settings.block_class_tags_tags : [];
			var plugins = (settings.block_class_tags_plugins) ? settings.block_class_tags_plugins : "";
			var extOptions = {plugins:plugins};
			var autocompleteList = [];

			// Add plugin configurations.
			if (plugins.length > 0) {
				var pluginList = plugins.split(" ");
				for (var i = pluginList.length - 1; i >= 0; i--) {
					switch (pluginList[i]) {
						case "tags":
						extOptions["tagsItems"] = tags;
						break;

						case "prompt":
						extOptions["prompt"] = "Add classes here...";
						break;

						case "autocomplete":
						$.get(settings.basePath + "block_class_tags/data.json", function (data) {
							autocompleteList = data;
							loadAutocomplete();
						});
						
						break;
					}
				};
			}

			function loadAutocomplete() {
				// Add default tags via autocomplete.
				$textextInput.bind('getSuggestions', function(e, data) {
					var list = autocompleteList,
					textext = $(e.target).textext()[0],
					query = (data ? data.query : '') || '';

					$(this).trigger(
						'setSuggestions',
						{ result : textext.itemManager().filter(list, query) }
						);
				});
			}

			// Check if jquery.textext.js is enabled.
			if ($.fn.textext) {
				var $textextInput = $(".form-item-css-class input");
				$textextInput.textext(extOptions);
				loadAutocomplete();
			}

		}
	};

})(jQuery, Drupal);