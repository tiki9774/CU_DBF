C---- Global variables for AVL plotting routines
C
C
      LOGICAL LPLOT, LCURS, LHID, LCREV
      COMMON /PLT_I/  LPLOT, LCURS, LHID, LCREV,
     &                IDEV, IDEVH, IPSLU,
     &                NCOLORS
C
      COMMON /PLT_R/  SCRNFRAC, SIZE, PLOTAR,
     &                XMIN,  XMAX,  YMIN, YMAX,
     &                XOFF,  YOFF,  SF,   CH,
     &                XMARG,YMARG,
     &                XPAGE,YPAGE,
     &                XWIND,YWIND,
     &               UNT(3,3), ORG(3,3), 
     &               PMARG,CPFAC, HNFAC, ENFAC,
     &               AZIMOB,ELEVOB,TILTOB,ROBINV
C


