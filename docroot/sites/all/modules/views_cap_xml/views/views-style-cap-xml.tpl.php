<edxlde:EDXLDistribution xmlns:edxlde="urn:oasis:names:tc:emergency:EDXL:DE:1.0">
  <edxlde:distributionID><?php print $options['edxlde:distributionID_prefix']?>:<?php print date('c')?></edxlde:distributionID>
  <edxlde:senderID><?php print $options['edxlde:senderID'] ?></edxlde:senderID>
  <edxlde:dateTimeSent><?php print date('c')?></edxlde:dateTimeSent>
  <edxlde:distributionStatus><?php print $options['edxlde:distributionStatus'] ?></edxlde:distributionStatus>
  <edxlde:distributionType><?php print $options['edxlde:distributionType'] ?></edxlde:distributionType>
  <edxlde:combinedConfidentiality><?php print $options['edxlde:combinedConfidentiality'] ?></edxlde:combinedConfidentiality>
  <edxlde:language><?php print $options['edxlde:language'] ?></edxlde:language>
    <?php print $rows; ?>
</edxlde:EDXLDistribution>


