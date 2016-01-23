<?php
/**
 * @file
 * Displays the XSL file related to MaPS Suite Log.
 *
 * Available variables:
 * - $page: The page content as Drupel element.
 *
 * @ingroup themeable
 */
?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
  <xsl:output method="html" encoding="utf-8" doctype-public="-//W3C//DTD XHTML 1.0 Transitional//EN" doctype-system="http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"/>

  <xsl:template name="get-context">
    <xsl:apply-templates select="//root/__content__"/>
  </xsl:template>

  <xsl:template match="/">
    <?php print $page; ?>
  </xsl:template>

  <!-- -->

  <xsl:template match="//root/introduction_message">
    <div id="introduction-message">
      <xsl:copy-of select="html/*"/>
    </div>
  </xsl:template>

  <!-- -->

  <xsl:template match="//root/__content__">
    <div id="content">
      <ul id="fold-links">
        <li><a id="fold-tree" href="javascript:;"><?php print t('Collapse all'); ?></a></li>
        <li><a id="unfold-tree" href="javascript:;"><?php print t('Expand all'); ?></a></li>
      </ul>
      <xsl:apply-templates select="child::context"/>
    </div>
  </xsl:template>

  <!-- -->

  <xsl:template match="context">
    <xsl:variable name="level" select="@level"/>
    <div class="context level-{$level}">
      <div class="context-title">
        <xsl:value-of select="@type"/>
        <xsl:choose>
          <xsl:when test="@id">
            id: <xsl:value-of select="@id"/>
          </xsl:when>
        </xsl:choose>
      </div>

      <div class="children-container">
        <xsl:apply-templates select="child::context"/>
        <xsl:apply-templates select="child::message"/>
      </div>
    </div>
  </xsl:template>

  <!-- -->

  <xsl:template match="message">
    <xsl:variable name="level" select="@level"/>
    <xsl:if test="text != '' or variables != '' or backtrace != '' or links != ''">
      <div class="message level-{$level}">

        <div class="message-content message-text">
          <xsl:value-of select="text"/>
        </div>

        <!-- Links -->
        <xsl:if test="links != ''">
          <div class="message-content message-links">
            <div class="message-title"><?php print t('Links'); ?></div>
            <ul class="children-container">
              <xsl:for-each select="links">
                <li>
                  <xsl:variable name="href" select="link"/>
                  <a href="{link}">
                    <xsl:value-of select="link"/>
                  </a>
                </li>
              </xsl:for-each>
            </ul>
          </div>
        </xsl:if>

        <!-- Variables -->
        <xsl:if test="variables != ''">
          <div class="message-content message-variables">
            <div class="message-title"><?php print t('Variables'); ?></div>
            <ul class="children-container">
              <xsl:for-each select="variables">
                <li>
                  <xsl:if test="variable/@name != ''">
                    <xsl:value-of select="variable/@name"/>:
                  </xsl:if>
                  <xsl:value-of select="variable"/>
                </li>
              </xsl:for-each>
            </ul>
          </div>
        </xsl:if>

        <!-- Backtrace -->
        <xsl:if test="backtrace!=''">
          <div class="message-content message-backtrace">
            <div class="message-title"><?php print t('Backtrace'); ?></div>
            <pre class="children-container">
              <xsl:value-of select="backtrace"/>
            </pre>
          </div>
        </xsl:if>

      </div>
    </xsl:if>
  </xsl:template>

</xsl:stylesheet>
