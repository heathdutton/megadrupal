(function ($) {
    Drupal.behaviors.commerceDressingRoom = {
        attach: function (context, settings) {
            // Array with all buttons positions.
            var buttonsPos = [60, 480];

            var timeOut, lastImageData;
            var canvasSource = $("#canvas-source")[0];
            var canvasBlended = $("#canvas-blended")[0];

            var contextSource = canvasSource.getContext('2d');
            var contextBlended = canvasBlended.getContext('2d');

            var buttons = [];

            // mirror video
            contextSource.translate(canvasSource.width, 0);
            contextSource.scale(-1, 1);

            var originalHeigth = $('.bjqs img').css('height');
            originalHeigth = originalHeigth.split('px');
            originalHeigth = originalHeigth[0];
            originalHeigth = parseInt(originalHeigth);
            var dynamicHeight = originalHeigth;

            if (hasGetUserMedia()) {
                $("#info").hide();
                $("#message").show();
            } else {
                $("#info").show();
                $("#message").hide();
                $video_demo = $("#video-demo");
                $video_demo.show();
                $video_demo[0].play();
                return;
            }

            var webcamError = function(e) {
                alert('Webcam error!', e);
            };

            var video = $('#webcam')[0];

            if (navigator.getUserMedia) {
                navigator.getUserMedia({audio: false, video: true}, function(stream) {
                    video.src = stream;
                    initialize();
                }, webcamError);
            } else if (navigator.webkitGetUserMedia) {
                navigator.webkitGetUserMedia({audio: false, video: true}, function(stream) {
                    video.src = window.webkitURL.createObjectURL(stream);
                    initialize();
                }, webcamError);
            } else {
                //video.src = 'somevideo.webm'; // fallback.
            }

            // Slideshow.
            $('#banner-fade').bjqs({
                height      : 400,
                width       : 320,
                showmarkers : false,
                animtype : 'slide',
                automatic: false,
                responsive  : false
            });

            function hasGetUserMedia() {
                // Opera builds are unprefixed.
                return !!(navigator.getUserMedia || navigator.webkitGetUserMedia ||
                    navigator.mozGetUserMedia || navigator.msGetUserMedia);
            }

            function initialize() {
                // 2 first Buttons
                var limit = 2;
                for (var i = 0; i < limit; i++) {
                    var button = {
                        ready: true,
                        visual: $("#button" + i)[0]
                    };
                    var heightButtonArea = 120;
                    var widthButtonArea = button.visual.width + 30;
                    button.area = {x:buttonsPos[i], y:0, width:widthButtonArea, height:heightButtonArea};
                    buttons.push(button);
                }
                start();
            }

            function playSound(obj) {
                if (!obj.ready) {
                    return;
                }

                var id = obj.visual.id;
                // Right.
                if (id == "button1") {
                    if (dynamicHeight >= originalHeigth) {
                        dynamicHeight = originalHeigth;
                    }
                    else {
                        dynamicHeight = dynamicHeight + 10;
                    }
                    $('.bjqs img').css({height:dynamicHeight});

                    $('.bjqs-next a').click();
                }
                // Left.
                else if(id == "button0") {
                    if (dynamicHeight <= 0) {
                        dynamicHeight = originalHeigth;
                    }
                    else {
                        dynamicHeight = dynamicHeight - 10;
                    }
                    $('.bjqs img').css({height:dynamicHeight});

                    $('.bjqs-prev a').click();
                }

                obj.ready = false;
                // throttle the button
                setTimeout(setButtonReady, 400, obj);
            }

            function setButtonReady(obj) {
                obj.ready = true;
            }

            function start() {
                $('.bjqs-slide img').show();
                $('.button-in-li').show();
                $('.bjqs-slide p').show();
                $('.bjqs img').css('display', 'block');
                $(canvasSource).show();
                $(canvasBlended).show();
                $("#btnregion").show();
                $("#message").hide();
                $("#description").show();
                update();
            }

            function update() {
                drawVideo();
                blend();
                checkAreas();
                timeOut = setTimeout(update, 1000 / 60);
            }

            function drawVideo() {
                contextSource.drawImage(video, 0, 0, video.width, video.height);
            }

            function blend() {
                var width = canvasSource.width;
                var height = canvasSource.height;
                // get webcam image data
                var sourceData = contextSource.getImageData(0, 0, width, height);
                // create an image if the previous image doesn’t exist
                if (!lastImageData) {
                    lastImageData = contextSource.getImageData(0, 0, width, height);
                }
                // create a ImageData instance to receive the blended result
                var blendedData = contextSource.createImageData(width, height);
                // blend the 2 images
                differenceAccuracy(blendedData.data, sourceData.data, lastImageData.data);
                // draw the result in a canvas
                contextBlended.putImageData(blendedData, 0, 0);
                // store the current webcam image
                lastImageData = sourceData;
            }

            function fastAbs(value) {
                // funky bitwise, equal Math.abs
                return (value ^ (value >> 31)) - (value >> 31);
            }

            function threshold(value) {
                return (value > 0x15) ? 0xFF : 0;
            }

            function differenceAccuracy(target, data1, data2) {
                if (data1.length != data2.length) {
                    return null;
                }
                var i = 0;
                while (i < (data1.length * 0.25)) {
                    var average1 = (data1[4 * i] + data1[4 * i + 1] + data1[4 * i + 2]) / 3;
                    var average2 = (data2[4 * i] + data2[4 * i + 1] + data2[4 * i + 2]) / 3;
                    var diff = threshold(fastAbs(average1 - average2));
                    target[4 * i] = diff;
                    target[4 * i + 1] = diff;
                    target[4 * i + 2] = diff;
                    target[4 * i + 3] = 0xFF;
                    ++i;
                }
            }

            function checkAreas() {
                // 2 first Buttons.
                var limit = 2;
                for (var r = 0; r < limit; r++) {
                    // get the pixels in a button area from the blended image
                    var blendedData = contextBlended.getImageData(buttons[r].area.x, buttons[r].area.y, buttons[r].area.width, buttons[r].area.height);
                    var i = 0;
                    var average = 0;
                    // loop over the pixels
                    while (i < (blendedData.data.length * 0.25)) {
                        // make an average between the color channel
                        average += (blendedData.data[i * 4] + blendedData.data[i * 4 + 1] + blendedData.data[i * 4 + 2]) / 3;
                        ++i;
                    }
                    // calculate an average between of the color values of the button area
                    average = Math.round(average / (blendedData.data.length * 0.25));
                    if (average > 10) {
                        // over a small limit, consider that a movement is detected
                        // play a button and show a visual feedback to the user
                        playSound(buttons[r]);
                        buttons[r].visual.style.display = "block";
                        $(buttons[r].visual).fadeOut();
                    }
                }
            }
        }
    };
})(jQuery);
