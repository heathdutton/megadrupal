
function drawRecommendationCarousel(json) {
    drawRecommendationCarouselToDiv(json, "recommenderDiv");
}

function drawRecommendationCarouselToDiv(json, recommenderDiv) {
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
            newRecommenderDiv = jQuery("<ul/>").addClass("horizontalcarousel").addClass("jcarousel-skin-default");
            for (x = 0; x < items.length; x++) {
                recommendedItem = jQuery("<li\>").append(
                    jQuery('<a/>').attr("href",items[x].url)
                        .append(jQuery("<img/>")
                        .attr("src",items[x].imageUrl)
                        .attr("alt",items[x].description)
                        .css("width","100%"))
                        .append(jQuery("<span/>").text(items[x].description))
                );

                newRecommenderDiv.append(recommendedItem);
            }
            jQuery("#"+recommenderDiv).empty();
            jQuery("#"+recommenderDiv).append(newRecommenderDiv);
            jQuery("#"+recommenderDiv+" .horizontalcarousel").jcarousel();
        }
    }
}