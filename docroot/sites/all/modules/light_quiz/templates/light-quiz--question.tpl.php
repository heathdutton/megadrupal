<script type="text/ng-template" id="templates/question.html">
  <h1><?php print t('Question !n of !total', array('!n' => '{{question.index + 1}}', '!total' => '{{question.total}}'))?></h1>

  <timer end-time="endTime" finish-callback="finished()">{{mminutes}}:{{sseconds}}</timer>

  <img ng-src="data:{{question.attachment_data.content_type}};base64,{{question.attachment_data.data}}" ng-if="getType(question.attachment_data) == 'image'">

  <p ng-if="getType(question.attachment_data) == 'text'" ng-bind-html="question.attachment_data.data"></p>

  <p ng-bind-html="question.question"></p>
  
  <form name="questionForm">
    <label ng-repeat="answer in question.answers">
      <input type="radio" name="userAnswer"
             ng-model="userAnswer"
             ng-value="$index"
             ng-click="setAnswer($index)" required>
      <span ng-bind-html="answer.value"></span>
    </label>
  </form>
  
  <button ng-click="next()"
          ng-if="question.index < (question.total - 1)"
          ng-disabled="questionForm.$invalid"><?php print t('Next Question') ?></button>

  <button ng-click="finish()"
          ng-if="question.index == (question.total - 1)"
          ng-disabled="questionForm.$invalid"><?php print t('Finish') ?></button>
</script>

