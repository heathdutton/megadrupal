/**
 * @file
 * Social Media functions needed for SocialMediaBar.
 */

(function($) {
    var actions = {
        'linkedin': 'share',
        'twitter': 'tweet',
        'googleplus': 'share'
    };

    $(document).ready(function() {
        // Links the share buttons to the ShareThis sharing mechanisms.
        $('.socialmediabar .share-button').click(function() {
            var network = $(this).parent('.network-container').attr('data-network');
            var url = document.URL;
            var title = document.title;

            var sections = title.split('|');
            var proxy_url = '/socialmediabar/shareproxy?network=' + network +
                '&url=' + url + '&title=' + sections[0];

            window.open(proxy_url, '_blank', 'menubar=no,width=500,height=500');

            if(typeof _gaq !== 'undefined' && typeof actions[network] !== 'undefined') {
                _gaq.push(['_trackSocial', network, actions[network], url]);
            }
        });

        // Checks the current counts per network.
        $('.socialmediabar .network-container').each(function() {
            var thisThis = this;
            var url = document.URL;
            var network = $(this).attr('data-network');
            $.ajax({
                data: {
                    'url': url,
                    'provider': network
                },
                success: function(data, status, xhr) {
                    $(thisThis).children('.count').html(data[network].outbound);
                },
                type: 'GET',
                url: '/socialmediabar/countproxy'
            });
        });

        // Updates the total count whenever and individual network count is updated.
        $('.socialmediabar .network-container .count').bind('DOMSubtreeModified', function() {
            var currentTotal = parseInt($('.total-container .count').html());
            if(isNaN(currentTotal)) {
                currentTotal = 0;
            }
            var currentCount = parseInt($(this).html());
            currentTotal += currentCount;
            $('.socialmediabar .total-container .count').html(currentTotal);
        });
    });
}(jQuery));
