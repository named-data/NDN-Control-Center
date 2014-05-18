<xsl:stylesheet version = '1.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match="/ndnd">
<fibs>
<xsl:apply-templates select="forwarding/fentry/dest" />
</fibs>
</xsl:template>

<xsl:template match="dest">
<fib>
<xsl:apply-templates select="faceid" />
<prefix><xsl:value-of select="../prefix"/></prefix>
</fib>
</xsl:template>

<xsl:template match="faceid">
<xsl:variable name="id"><xsl:value-of select="." /></xsl:variable>
<faceID><xsl:copy-of select="$id" /></faceID>
<ip>
<xsl:choose>
  <xsl:when test="count(//face/ip[../faceid=$id]) &gt; 0"><xsl:value-of select="//face/ip[../faceid=$id]" />
  </xsl:when>
  <xsl:otherwise>app</xsl:otherwise>
</xsl:choose>
</ip>
</xsl:template>

</xsl:stylesheet>
