<xsl:stylesheet version = '1.0'
                xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>

<xsl:template match="/">
<stats>
  <data>
    <in><xsl:value-of select="sum(/ndnd/faces/face/meters/datain/total)" /></in>
    <out><xsl:value-of select="sum(/ndnd/faces/face/meters/dataout/total)" /></out>
    <!-- <in><xsl:value-of select="sum(/ndnd/faces/face/meters/datain/persec)" /></in> -->
    <!-- <out><xsl:value-of select="sum(/ndnd/faces/face/meters/dataout/persec)" /></out> -->
  </data>
  <interests>
    <in><xsl:value-of select="sum(/ndnd/faces/face/meters/intrin/total)" /></in>
    <out><xsl:value-of select="sum(/ndnd/faces/face/meters/introut/total)" /></out>
    <!-- <in><xsl:value-of select="sum(/ndnd/faces/face/meters/intrin/persec)" /></in> -->
    <!-- <out><xsl:value-of select="sum(/ndnd/faces/face/meters/introut/persec)" /></out> -->
  </interests>
</stats>
</xsl:template>
</xsl:stylesheet>
