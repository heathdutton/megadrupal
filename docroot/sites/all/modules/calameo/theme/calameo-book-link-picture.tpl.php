<?php
/**
 * In addition to regular field template variables, the following have been added from the Calameo book record:
 *
 * AccountID        String    Account ID that created this publication
 * AllowMini        Integer   Sends 1 if the publication allows access to the miniCalamÃ©o and 0 if not.
 * AuthID           String    Authentication parameter for private URLs (authid).
 * Category         String    Category.
 * Comments         String    Absolute URL for the publication's comments
 * Creation         Datetime  Date of creation of the publication
 * Date             Date      Date of citation of the publication.
 * Description      String    Description of the publication.
 * Dialect          String    Language.
 * Downloads        Integer   Number of downloads (?)
 * Favorites        Integer   Number of favorites (?)
 * Format           String    Format.
 * Height           Integer   Height of a page of the publication.
 * ID               String    ID of the publication
 * IsPrivate        Integer   Sends 1 if the publication is private and 0 if not.
 * IsPublished      Integer   Sends 1 if the publication is published and 0 if not.
 * Modification     Datetime  Date of the last modification of the publication.
 * Name             String    Title of the publication.
 * Pages            Integer   Number of pages of the publication.
 * PictureUrl       String    Absolute URL for the publication's cover
 * PublicUrl        String    Absolute URL for the publication's overview.
 * Status           String    Conversion status of the publication. Either QUEUE (waiting to be converted), PROCESS (processing document), STORE (converting document), ERROR (error during convertion) or DONE (publication ready).
 * SubscriptionID   String    Sends 1 if the publication is private and 0 if not.
 * ThumbUrl         String    Absolute URL for the publication's thumbnail.
 * Views            Integer   Number of views (?)
 * ViewUrl          String    Absolute URL for the publication's reading page
 * Width            Integer   Width of a page of the publication.
 *
 * There are also some derived variables available that are already formatted into HTML:
 *
 * ThumbImg           IMG element for ThumbUrl
 * PictureImg         IMG element for PictureUrl
 * NameViewerLink     Name linked to ViewUrl
 * ThumbViewerLink    ThumbUrl linked to ViewUrl
 * PictureViewerLink  PictureUrl linked to ViewUrl
 * NamePublicLink     Name linked to PublicUrl
 * ThumbPublicLink    ThumbUrl linked to PublicUrl
 * PicturePublicLink  PictureUrl linked to PublicUrl
 *
 * Display settings passed through:
 *
 * target             Link target attribute
 */
?>
<div class="calameo-book-link-picture">
  <?php print $PictureViewerLink; ?>
</div>

