<?php
/**
 * @file
 * Generates history table for case tracker.
 */
?>

<div id="table">
  <table>
      <th style="color:green;">Date of Entry</th>
      <th>Current Status</th>
      <th>Timeline Step</th>
      <th>Correspondance From</th>
      <th>Email Conversation</th>
      <th>Phone Conversation</th>
      <th>Attachment?</th>
    <?php
      foreach ($data as $key => $details):
      ?>
        <tr>
        <?php
        foreach ($details as $col => $val):
        ?>
          <td>
            <?php 
            if ($col == 'attachment' && $val == 'yes'):
              echo l($val, '');
            elseif ($col == 'date_of_entry'):
              echo date('jS M Y', $val);
            elseif ($val == ''):
              echo '-';
            else:
              echo $val;
            endif;
            ?>
          </td>
      <?php
        endforeach;
      ?>
        </tr>
      <?php
      endforeach;
    ?>
  </table>
</div>
