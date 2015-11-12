<?php

/**
 * @file
 * Default theme implementation to display an error message.
 *
 * Available variables:
 * - $errors: An array of errors.
 * - $empty: empty string to display when no errors are found.
 *
 * @see template_preprocess()
 * @see template_process()
 *
 * @ingroup themeable
 */
?>
<table width="100%" id="error-details">
  <tbody>
    <?php if(!empty($errors)):?>
    <tr>
      <td width="30%">Where</td>
      <td width="70%" class="lineAndFile">
        <div>
          <?php echo implode($errors['where'], '<br/>')?>
        </div>
      </td>
    </tr>
    <tr>
      <td>Browser</td>
      <td>
        <div id="browsers">
          <span class="highlight"><?php echo implode($errors['browser'], '<br/>')?>
          </span>.
        </div>
      </td>
    </tr>
    <tr>
      <td>Platform</td>
      <td>
        <div id="platform">
          <span class="highlight"><?php echo implode($errors['platform'], '<br/>')?>
          </span>.
        </div>
      </td>
    </tr>
    <tr>
      <td>On page</td>
      <td>
        <ul class="error-pages">
          <?php foreach($errors['page'] as $page):?>
          <li><?php echo l($page, $page, array('attributes' => array('target' => 'blank')))?>
          </li>
          <?php endforeach;?>
        </ul>
      </td>
    </tr>
    <tr>
      <td>Occurrences</td>
      <td>
        <div id="occurrence-data">
          <ul id="occurrences">
            <?php foreach($errors['occurrence'] as $occurrence):?>
            <li>
              <dl>
                <dt>Message</dt>
                <dd>
                  <?php echo $occurrence['message']?>
                </dd>
                <dt>Page</dt>
                <dd>
                  <?php echo l($occurrence['page'], $occurrence['page'], array('attributes' => array('target' => 'blank', 'title' => $occurrence['message'])))?>
                </dd>
                <dt>UA string</dt>
                <dd>
                  <?php echo $occurrence['user_agent']?>
                </dd>
                <dt>When</dt>
                <dd>
                  <?php echo $occurrence['pageload'] ? t("After Page Load") : t("Before Page Load") ?>
                </dd>
                <dt>Date</dt>
                <dd>
                  <?php echo format_date($occurrence['timestamp'], 'long')?>
                </dd>
              </dl>
            </li>
            <?php endforeach;?>
          </ul>
        </div>
      </td>
    </tr>
    <?php else:?>
    <tr>
      <p>
        <?php echo $empty ?>
      </p>
    </tr>
    <?php endif;?>
  </tbody>
</table>
