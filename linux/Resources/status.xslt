<xsl:stylesheet version = '2.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match="/ndnd">
<html><body>
<xsl:apply-templates select="forwarding/fentry" />
</body></html>
</xsl:template>

<xsl:template match="fentry">
<xsl:apply-templates select="dest/faceid" />
<xsl:text> </xsl:text>
<xsl:value-of select="prefix"/>
<br/>
</xsl:template>

<xsl:template match="faceid">
<xsl:variable name="id"><xsl:value-of select="." /></xsl:variable>
<b>face: <xsl:copy-of select="$id" />

<xsl:choose>
  <xsl:when test="count(//face/ip[../faceid=$id]) &gt; 0"> (<xsl:value-of select="//face/ip[../faceid=$id]" />)
  </xsl:when>
  <xsl:otherwise> (app)</xsl:otherwise>
</xsl:choose></b>
</xsl:template>

</xsl:stylesheet>
