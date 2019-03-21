WORDS THAT SHOULD RHYME
To push someone violently & to take something away
shove & remove
The highest singing voice & a fissure in the earth's crust
soprano & volcano
To send someone away forever & a breakfast pastry
banish & Danish
A watercraft & a person such as Prime Minister Tihomir Oreskovic
a boat & a Croat
Full-grown & the physical world including all living things
mature & nature
PLAYTIME WITH SHAKESPEARE
"The M of V"
The Merchant of Venice
"M for M"
Measure for Measure
With action at Agincourt, "H the F"
Henry V
"A Y L I"|*|As You Like It
"T of A"|*|Timon of Athens
*SPOT THE CANADIAN*
Q1|*|A1
Q2|*|A2
Q3|*|A3
Q4|*|A4
Q5|*|A5
*CONTAINERS*
Q1|*|A
Q2|*|A
Q3|*|A
Q4|*|A
Q5|*|A
*U.S. GEOGRAPHY*
Q1|*|A
Q2|*|A
Q3|*|A
Q4|*|A
Q5|*|A
*AUDRA McDONALD ON BROADWAY*
Q|*|A
Q|*|A
Q|*|A
Q|*|A
Q|*|A






int di = (width - 2) / 6;
        int hi = (height - 8) / 6;
        int padding = (width - ((di)*6))/2;
        int realWidth = di * 6;
        int realHeight = hi * 6;
        int widthDiv = (realWidth - 2) / 6;
        int heightDiv = (realHeight) / 6;
        VERTICAL_PADDING(2);
        center("JEOPARDY GAME BOARD", width, BG_BLUE_FG_WHITE, 0, 0);
        VERTICAL_PADDING(2);
        // center("ETHAN, you are picking.", width, BG_BLUE_FG_WHITE, 0, 0);
        HORIZONTAL_PADDING(padding);
        printf("╔");
        for (int i = 0; i < realWidth - 2; i++) {
                if (i % widthDiv == 0 && i != 0 && (i != (widthDiv * 6))) { printf("╤"); } else { printf("═"); }
        }
        printf("╗");
        HORIZONTAL_PADDING(padding);
        printf("\n");
        for (int i = 0; i < heightDiv; i++) {
                HORIZONTAL_PADDING(padding);
                printf("║");
                int isPrintTitle = 0;
                for (int j = 0; j < realWidth - 2; j++) {
                        if (j % widthDiv == 0 && j != 0 && (j != (widthDiv * 6))) { 
                                printf("│"); 
                                isPrintTitle = 0;
                        } else { 
                                if (isPrintTitle != 1) {
                                        if (i == 3) {
                                                isPrintTitle = 1;
                                                centerNoNewline(" CAT ", widthDiv - 1, BG_BLUE_FG_WHITE, 0, widthDiv);
                                        } else {
                                                printf(" "); 
                                        }
                                }
                        }
                }
                printf("║");
                HORIZONTAL_PADDING(padding);
                printf("\n");
        }
        HORIZONTAL_PADDING(padding);
        printf("╟");
        for (int j = 0; j < realWidth - 2; j++) {
                if (j % widthDiv == 0 && j != 0 && (j != (widthDiv * 6))) { printf("┼"); } else { printf("─"); }
        }
        printf("╢");
        HORIZONTAL_PADDING(padding);
        printf("\n");
        for (int k = 0; k < 5; k++) {
                for (int i = 0; i < heightDiv; i++) {
                        HORIZONTAL_PADDING(padding);
                        printf("║");
                        for (int j = 0; j < realWidth - 2; j++) {
                                if (j % widthDiv == 0 && j != 0 && (j != (widthDiv * 6))) { printf("│"); } else { printf(" "); }
                        }
                        printf("║");
                        HORIZONTAL_PADDING(padding);
                        printf("\n");
                }
                if (k != 4) {
                        HORIZONTAL_PADDING(padding);
                        printf("╟");
                        for (int j = 0; j < realWidth - 2; j++) {
                                if (j % widthDiv == 0 && j != 0 && (j != (widthDiv * 6))) { printf("┼"); } else { printf("─"); }
                        }
                        printf("╢");
                        HORIZONTAL_PADDING(padding);
                        printf("\n");
                }
        }
        HORIZONTAL_PADDING(padding);
        printf("╚");
        for (int i = 0; i < realWidth - 2; i++) {
                if (i % widthDiv == 0 && i != 0 && (i != (widthDiv * 6))) { printf("╧"); } else { printf("═"); }
        }
        printf("╝");
        HORIZONTAL_PADDING(padding);