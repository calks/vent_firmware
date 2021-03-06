//******************************************************************   
//* REALTEK CONTROL REGISTER OFFSETS   
//*   All offsets in Page 0 unless otherwise specified   
//*   All functions accessing CR must leave CR in page 0 upon exit   
//******************************************************************   
#define CR          0x00   
#define PSTART      0x01   
#define PAR0        0x01    // Page 1   
#define CR9346      0x01    // Page 3   
#define PSTOP       0x02   
#define BNRY        0x03   
#define TSR         0x04   
#define TPSR        0x04   
#define TBCR0       0x05   
#define NCR         0x05   
#define TBCR1       0x06   
#define _ISR         0x07   
#define CURR        0x07   // Page 1   
#define RSAR0       0x08   
#define CRDA0       0x08   
#define RSAR1       0x09   
#define CRDA1       0x09   
#define RBCR0       0x0A   
#define RBCR1       0x0B   
#define RSR         0x0C   
#define RCR         0x0C   
#define TCR         0x0D   
#define CNTR0       0x0D   
#define DCR         0x0E   
#define CNTR1       0x0E   
#define IMR         0x0F   
#define CNTR2       0x0F   
#define RDMAPORT    0x10   
#define RSTPORT     0x18   
#define CONFIG2     0x05    // page 3   
#define CONFIG3     0x06    // page 3   
#define RTL_EECR        0x01    // page 3   
   
   
   
/*****************************************************************************   
*   
* RTL ISR Register Bits   
*   
*****************************************************************************/   
#define ISR_RST 7   
#define ISR_OVW 4   
#define ISR_PRX 0   
#define ISR_RDC 6   
#define ISR_PTX 1   
   
   
/*****************************************************************************  
*  
*  RTL Register Initialization Values  
*  
*****************************************************************************/   
// RCR : accept broadcast packets and packets destined to this MAC   
//         drop short frames and receive errors   
#define RCR_INIT        0x04   
   
// TCR : default transmit operation - CRC is generated    
#define TCR_INIT        0x00   
   
// DCR : allows send packet to be used for packet retreival   
//         FIFO threshold: 8-bits (works)   
//         8-bit transfer mode   
#define DCR_INIT        0x58   
   
// IMR : interrupt enabled for receive and overrun events   
#define IMR_INIT        0x11   
   
// buffer boundaries - transmit has 6 256-byte pages   
//   receive has 26 256-byte pages   
//   entire available packet buffer space is allocated   
#define TXSTART_INIT    0x40   
#define RXSTART_INIT    0x46   
#define RXSTOP_INIT     0x60   














#define NIC_CR 0x00        /*!< \brief Command register */
#define NIC_IOPORT 0x10    /*!< \brief I/O data port */
#define NIC_RESET 0x1f     /*!< \brief Reset port */

/*
 * Page 0 register offsets.
 */
#define NIC_PG0_CLDA0 0x01    /*!< \brief Current local DMA address 0 */
#define NIC_PG0_PSTART 0x01   /*!< \brief Page start register */
#define NIC_PG0_CLDA1 0x02    /*!< \brief Current local DMA address 1 */
#define NIC_PG0_PSTOP 0x02    /*!< \brief Page stop register */
#define NIC_PG0_BNRY 0x03     /*!< \brief Boundary pointer */
#define NIC_PG0_TSR 0x04      /*!< \brief Transmit status register */
#define NIC_PG0_TPSR 0x04     /*!< \brief Transmit page start address */
#define NIC_PG0_NCR 0x05      /*!< \brief Number of collisions register */
#define NIC_PG0_TBCR0 0x05    /*!< \brief Transmit byte count register 0 */
#define NIC_PG0_FIFO 0x06     /*!< \brief FIFO */
#define NIC_PG0_TBCR1 0x06    /*!< \brief Transmit byte count register 1 */
#define NIC_PG0_ISR 0x07      /*!< \brief Interrupt status register */
#define NIC_PG0_CRDA0 0x08    /*!< \brief Current remote DMA address 0 */
#define NIC_PG0_RSAR0 0x08    /*!< \brief Remote start address register 0 
                                   Low byte address to read from the buffer. */
#define NIC_PG0_CRDA1 0x09    /*!< \brief Current remote DMA address 1 */
#define NIC_PG0_RSAR1 0x09    /*!< \brief Remote start address register 1 
                                   High byte address to read from the buffer. */
#define NIC_PG0_RBCR0 0x0a    /*!< \brief Remote byte count register 0 
                                   Low byte of the number of bytes to read
                                   from the buffer. */
#define NIC_PG0_RBCR1 0x0b    /*!< \brief Remote byte count register 1
                                   High byte of the number of bytes to read
                                   from the buffer. */
#define NIC_PG0_RSR 0x0c      /*!< \brief Receive status register */
#define NIC_PG0_RCR 0x0c      /*!< \brief Receive configuration register */
#define NIC_PG0_CNTR0 0x0d    /*!< \brief Tally counter 0 (frame alignment errors) */
#define NIC_PG0_TCR 0x0d      /*!< \brief Transmit configuration register */
#define NIC_PG0_CNTR1 0x0e    /*!< \brief Tally counter 1 (CRC errors) */
#define NIC_PG0_DCR 0x0e      /*!< \brief Data configuration register */
#define NIC_PG0_CNTR2 0x0f    /*!< \brief Tally counter 2 (Missed packet errors) */
#define NIC_PG0_IMR 0x0f      /*!< \brief Interrupt mask register */

/*
 * Page 1 register offsets.
 */
#define NIC_PG1_PAR0 0x01     /*!< \brief Physical address register 0 */
#define NIC_PG1_PAR1 0x02     /*!< \brief Physical address register 1 */
#define NIC_PG1_PAR2 0x03     /*!< \brief Physical address register 2 */
#define NIC_PG1_PAR3 0x04     /*!< \brief Physical address register 3 */
#define NIC_PG1_PAR4 0x05     /*!< \brief Physical address register 4 */
#define NIC_PG1_PAR5 0x06     /*!< \brief Physical address register 5 */
#define NIC_PG1_CURR 0x07     /*!< \brief Current page register
                                   The next incoming packet will be stored
                                   at this page address. */
#define NIC_PG1_MAR0 0x08     /*!< \brief Multicast address register 0 */
#define NIC_PG1_MAR1 0x09     /*!< \brief Multicast address register 1 */
#define NIC_PG1_MAR2 0x0a     /*!< \brief Multicast address register 2 */
#define NIC_PG1_MAR3 0x0b     /*!< \brief Multicast address register 3 */
#define NIC_PG1_MAR4 0x0c     /*!< \brief Multicast address register 4 */
#define NIC_PG1_MAR5 0x0d     /*!< \brief Multicast address register 5 */
#define NIC_PG1_MAR6 0x0e     /*!< \brief Multicast address register 6 */
#define NIC_PG1_MAR7 0x0f     /*!< \brief Multicast address register 7 */

/*
 * Page 2 register offsets.
 */
#define NIC_PG2_PSTART 0x01   /*!< \brief Page start register */
#define NIC_PG2_CLDA0 0x01    /*!< \brief Current local DMA address 0 */
#define NIC_PG2_PSTOP 0x02    /*!< \brief Page stop register */
#define NIC_PG2_CLDA1 0x02    /*!< \brief Current local DMA address 1 */
#define NIC_PG2_RNP 0x03      /*!< \brief Remote next packet pointer */
#define NIC_PG2_TSPR 0x04     /*!< \brief Transmit page start register */
#define NIC_PG2_LNP 0x05      /*!< \brief Local next packet pointer */
#define NIC_PG2_ACU 0x06      /*!< \brief Address counter (upper) */
#define NIC_PG2_ACL 0x07      /*!< \brief Address counter (lower) */
#define NIC_PG2_RCR 0x0c      /*!< \brief Receive configuration register */
#define NIC_PG2_TCR 0x0d      /*!< \brief Transmit configuration register */
#define NIC_PG2_DCR 0x0e      /*!< \brief Data configuration register */
#define NIC_PG2_IMR 0x0f      /*!< \brief Interrupt mask register */

/*
 * Page 3 register offsets.
 */
#define NIC_PG3_EECR     0x01    /*!< \brief EEPROM command register */
#define NIC_PG3_BPAGE    0x02    /*!< \brief Boot-ROM page register */
#define NIC_PG3_CONFIG0  0x03    /*!< \brief Configuration register 0 (r/o) */
#define NIC_PG3_CONFIG1  0x04    /*!< \brief Configuration register 1 */
#define NIC_PG3_CONFIG2  0x05    /*!< \brief Configuration register 2 */
#define NIC_PG3_CONFIG3  0x06    /*!< \brief Configuration register 3 */
#define NIC_PG3_CSNSAV   0x08    /*!< \brief CSN save register (r/o) */
#define NIC_PG3_HLTCLK   0x09    /*!< \brief Halt clock */
#define NIC_PG3_INTR     0x0b    /*!< \brief Interrupt pins (r/o) */

/*
 * Command register bits.
 */
#define NIC_CR_STP 0x01    /*!< \brief Stop */
#define NIC_CR_STA 0x02    /*!< \brief Start */
#define NIC_CR_TXP 0x04    /*!< \brief Transmit packet */
#define NIC_CR_RD0 0x08    /*!< \brief Remote DMA command bit 0 */
#define NIC_CR_RD1 0x10    /*!< \brief Remote DMA command bit 1 */
#define NIC_CR_RD2 0x20    /*!< \brief Remote DMA command bit 2 */
#define NIC_CR_PS0 0x40    /*!< \brief Page select bit 0 */
#define NIC_CR_PS1 0x80    /*!< \brief Page select bit 1 */

/*
 * Interrupt status register bits.
 */
#define NIC_ISR_PRX 0x01      /*!< \brief Packet received */
#define NIC_ISR_PTX 0x02      /*!< \brief Packet transmitted */
#define NIC_ISR_RXE 0x04      /*!< \brief Receive error */
#define NIC_ISR_TXE 0x08      /*!< \brief Transmit error */
#define NIC_ISR_OVW 0x10      /*!< \brief Overwrite warning */
#define NIC_ISR_CNT 0x20      /*!< \brief Counter overflow */
#define NIC_ISR_RDC 0x40      /*!< \brief Remote DMA complete */
#define NIC_ISR_RST 0x80      /*!< \brief Reset status */

/*
 * Interrupt mask register bits.
 */
#define NIC_IMR_PRXE 0x01     /*!< \brief Packet received interrupt enable */
#define NIC_IMR_PTXE 0x02     /*!< \brief Packet transmitted interrupt enable */
#define NIC_IMR_RXEE 0x04     /*!< \brief Receive error interrupt enable */
#define NIC_IMR_TXEE 0x08     /*!< \brief Transmit error interrupt enable */
#define NIC_IMR_OVWE 0x10     /*!< \brief Overwrite warning interrupt enable */
#define NIC_IMR_CNTE 0x20     /*!< \brief Counter overflow interrupt enable */
#define NIC_IMR_RCDE 0x40     /*!< \brief Remote DMA complete interrupt enable */

/*
 * Data configuration register bits.
 */
#define NIC_DCR_WTS 0x01      /*!< \brief Word transfer select */
#define NIC_DCR_BOS 0x02      /*!< \brief Byte order select */
#define NIC_DCR_LAS 0x04      /*!< \brief Long address select */
#define NIC_DCR_LS 0x08       /*!< \brief Loopback select */
#define NIC_DCR_AR 0x10       /*!< \brief Auto-initialize remote */
#define NIC_DCR_FT0 0x20      /*!< \brief FIFO threshold select bit 0 */
#define NIC_DCR_FT1 0x40      /*!< \brief FIFO threshold select bit 1 */

/*
 * Transmit configuration register bits.
 */
#define NIC_TCR_CRC 0x01      /*!< \brief Inhibit CRC */
#define NIC_TCR_LB0 0x02      /*!< \brief Encoded loopback control bit 0 */
#define NIC_TCR_LB1 0x04      /*!< \brief Encoded loopback control bit 1 */
#define NIC_TCR_ATD 0x08      /*!< \brief Auto transmit disable */
#define NIC_TCR_OFST 0x10     /*!< \brief Collision offset enable */

/*
 * Transmit status register bits.
 */
#define NIC_TSR_PTX 0x01      /*!< \brief Packet transmitted */
#define NIC_TSR_COL 0x04      /*!< \brief Transmit collided */
#define NIC_TSR_ABT 0x08      /*!< \brief Transmit aborted */
#define NIC_TSR_CRS 0x10      /*!< \brief Carrier sense lost */
#define NIC_TSR_FU 0x20       /*!< \brief FIFO underrun */
#define NIC_TSR_CDH 0x40      /*!< \brief CD heartbeat */
#define NIC_TSR_OWC 0x80      /*!< \brief Out of window collision */

/*
 * Receive configuration register bits.
 */
#define NIC_RCR_SEP 0x01      /*!< \brief Save errored packets */
#define NIC_RCR_AR 0x02       /*!< \brief Accept runt packets */
#define NIC_RCR_AB 0x04       /*!< \brief Accept broadcast */
#define NIC_RCR_AM 0x08       /*!< \brief Accept multicast */
#define NIC_RCR_PRO 0x10      /*!< \brief Promiscuous physical */
#define NIC_RCR_MON 0x20      /*!< \brief Monitor mode */

/*
 * Receive status register bits.
 */
#define NIC_RSR_PRX 0x01      /*!< \brief Packet received intact */
#define NIC_RSR_CRC 0x02      /*!< \brief CRC error */
#define NIC_RSR_FAE 0x04      /*!< \brief Frame alignment error */
#define NIC_RSR_FO 0x08       /*!< \brief FIFO overrun */
#define NIC_RSR_MPA 0x10      /*!< \brief Missed packet */
#define NIC_RSR_PHY 0x20      /*!< \brief Physical/multicast address */
#define NIC_RSR_DIS 0x40      /*!< \brief Receiver disabled */
#define NIC_RSR_DFR 0x80      /*!< \brief Deferring */

/*
 * EEPROM command register bits.
 */
#define NIC_EECR_EEM1  0x80    /*!< \brief EEPROM Operating Mode */
#define NIC_EECR_EEM0  0x40    /*!< \brief  EEPROM Operating Mode
                                    - 0 0 Normal operation
                                    - 0 1 Auto-load 
                                    - 1 0 9346 programming 
                                    - 1 1 Config register write enab */
#define NIC_EECR_EECS  0x08    /*!< \brief EEPROM Chip Select */
#define NIC_EECR_EESK  0x04    /*!< \brief EEPROM Clock */
#define NIC_EECR_EEDI  0x02    /*!< \brief EEPROM Data In */
#define NIC_EECR_EEDO  0x01    /*!< \brief EEPROM Data Out */

/*
 * Configuration register 2 bits.
 */
#define NIC_CONFIG2_PL1   0x80 /*!< \brief Network media type */
#define NIC_CONFIG2_PL0   0x40 /*!< \brief Network media type
                                    - 0 0 TP/CX auto-detect 
                                    - 0 1 10baseT 
                                    - 1 0 10base5 
                                    - 1 1 10base2 */
#define NIC_CONFIG2_BSELB 0x20 /*!< \brief BROM disable */
#define NIC_CONFIG2_BS4   0x10 /*!< \brief BROM size/base */
#define NIC_CONFIG2_BS3   0x08
#define NIC_CONFIG2_BS2   0x04
#define NIC_CONFIG2_BS1   0x02
#define NIC_CONFIG2_BS0   0x01

/*
 * Configuration register 3 bits
 */
#define NIC_CONFIG3_PNP     0x80 /*!< \brief PnP Mode */
#define NIC_CONFIG3_FUDUP   0x40 /*!< \brief Full duplex */
#define NIC_CONFIG3_LEDS1   0x20 /*!< \brief LED1/2 pin configuration 
                                        - 0 LED1 == LED_RX, LED2 == LED_TX 
                                        - 1 LED1 == LED_CRS, LED2 == MCSB */
#define NIC_CONFIG3_LEDS0   0x10 /*!< \brief LED0 pin configration
                                        - 0 LED0 pin == LED_COL 
                                        - 1 LED0 pin == LED_LINK */
#define NIC_CONFIG3_SLEEP   0x04 /*!< \brief Sleep mode */
#define NIC_CONFIG3_PWRDN   0x02 /*!< \brief Power Down */
#define NIC_CONFIG3_ACTIVEB 0x01 /*!< \brief inverse of bit 0 in PnP Act Reg */