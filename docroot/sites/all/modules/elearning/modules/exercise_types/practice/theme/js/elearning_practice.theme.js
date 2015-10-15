/**
 * Jquery timer
 */

(function($) {
  Drupal.theme.prototype.elearningCheckAnswerImmediately = function (answer, question) {
    $(question).removeClass('incorrect correct not-answered');
    $(question).addClass(answer);

    $('audio').each(function () {
        $(this).trigger('pause').prop("currentTime",0);
    });

    if($('.correct-answer').length === 0){
      var basePath = this.settings.basePath;
      var elearingPracticePath = this.settings.ElearningPracticePath;
      var correctPath = basePath + elearingPracticePath + '/theme/audio/correct.mp3';
      var incorrectPath = basePath + elearingPracticePath + '/theme/audio/incorrect.mp3';
      $('body').append('<audio class="correct-answer"><source src="' + correctPath + '"  type="audio/mpeg"/></audio>');
      $('body').append('<audio class="incorrect-answer"><source src="' + incorrectPath + '"  type="audio/mpeg"/></audio>');
    }
    if(answer === "correct"){
      $('audio.correct-answer').trigger('play');
    }
     if(answer === "incorrect"){
      $('audio.incorrect-answer').trigger('play');
    }

    // Trigger an event so any custom javascript may act upon it.
    $(question).trigger('questionAnswered');

  };
})(jQuery);
