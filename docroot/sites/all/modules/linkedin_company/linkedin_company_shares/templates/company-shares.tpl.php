<?php
/**
 * @file
 * Template file for the LinkedIn Company Status module recent updates block.
 */

  /**
   * Copy to your theme folder if you wish to modify.
   *
   * Available variables:
   *
   * $shares
   *   - An associative array of the following data from LinkedIn
   *   $company_name - The company name
   *   $company_id - The company ID
   *   $timestamp - The time the status was generated
   *   $submitted-url - The direct link to the post
   *   $shortened-url - A shortened version of the direct link
   *   $comment - The comment attached to the post
   *   $has_attachment - Does the post have an attachment to it (i.e a link)
   *   $title - The post title
   *   $description - The content of the post
   *   $submitted-image-url - The URL to the image submitted with the post
   *   $thumbnail-url - The URL of the post thumbnail
   *   $eyebrow-url - A link to the posts content
   */
?>
<?php foreach ($shares as $share): ?>
  <div class="share">
    <?php if($share['comment']): ?>
    <p class="share-comment"><?php print $share['comment']; ?></p>
    <?php endif; ?>

    <?php if ($share['has_attachment']): ?>
      <div class="attachment">
        <img class="attachment-image"
             src="<?php print $share['thumbnail-url']; ?>"/>

        <p class="attachment-title">
          <?php print l($share['title'],
            $share['eyebrow-url'],
            array(
              'attributes' =>
              array(
                'rel'    => 'nofollow',
                'target' => '_blank',
              ),
            ));
          ?>
        </p>

        <p class="attachment-description">
          <?php print $share['description'] ?>
        </p>
      </div>
    <?php endif; ?>
  </div>
<?php endforeach; ?>
