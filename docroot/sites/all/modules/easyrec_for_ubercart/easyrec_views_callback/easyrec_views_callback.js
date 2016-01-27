
function drawRecommendationWithDrupalViewsToDiv(json, recommenderDiv) {
    if ("undefined" == typeof(json.error)) { // if no error show recommendations

        try {
            var items = json.recommendeditems.item;
        } catch(e) {
            return;
        }

        /* when the object is already in array format, this block will not execute */
        if ("undefined" == typeof(items.length)) {
            items = new Array(items);
        }

        // display recommendations in the DIV layer 'recommendation'
        if (items.length > 0) {
            ids =  "";
            for (x = 0; x < items.length; x++) {
				ids = ids + items[x].id + ",";
            }
			
            jQuery("#"+recommenderDiv).load(Drupal.settings.basePath + "easyrec/recommendations/" + ids);
        }
		
    }
}