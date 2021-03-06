/*
 *  Light.c
 *  Brogue
 *
 *  Created by Brian Walker on 1/21/09.
 *  Copyright 2012. All rights reserved.
 *  
 *  This file is part of Brogue.
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as
 *  published by the Free Software Foundation, either version 3 of the
 *  License, or (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <math.h>
#include "Rogue.h"
#include "IncludeGlobals.h"

void logLights() {
    
    short i, j;
    
    printf("    ");
    for (i=0; i<COLS-2; i++) {
        printf("%i", i % 10);
    }
    printf("\n");
    for( j=0; j<DROWS-2; j++ ) {
        if (j < 10) {
            printf(" ");
        }
        printf("%i: ", j);
        for( i=0; i<DCOLS-2; i++ ) {
            if (tmap[i][j].light[0] == 0) {
                printf(" ");
            } else {
                printf("%i", MAX(0, tmap[i][j].light[0] / 10 - 1));
            }
        }
        printf("\n");
    }
    printf("\n");
}

// Returns true if any part of the light hit cells that are in the player's field of view.
bool paintLight(lightSource *theLight, short x, short y, bool isMinersLight, bool maintainShadows) {
    short i, j, k;
    short colorComponents[3], randComponent, lightMultiplier;
    short fadeToPercent;
    double radius;
    char grid[DCOLS][DROWS];
    bool dispelShadows, overlappedFieldOfView;
    
    brogueAssert(rogue.RNG == RNG_SUBSTANTIVE);
    
    radius = randClump(theLight->lightRadius);
    radius /= 100;
    
    randComponent = rand_range(0, theLight->lightColor->rand);
    colorComponents[0] = randComponent + theLight->lightColor->red + rand_range(0, theLight->lightColor->redRand);
    colorComponents[1] = randComponent + theLight->lightColor->green + rand_range(0, theLight->lightColor->greenRand);
    colorComponents[2] = randComponent + theLight->lightColor->blue + rand_range(0, theLight->lightColor->blueRand);
    
    // the miner's light does not dispel IS_IN_SHADOW,
    // so the player can be in shadow despite casting his own light.
    dispelShadows = !maintainShadows && (colorComponents[0] + colorComponents[1] + colorComponents[2]) > 0;
    
    fadeToPercent = theLight->radialFadeToPercent;
    
    // zero out only the relevant rectangle of the grid
    for (i = MAX(0, x - (radius + FLOAT_FUDGE)); i < DCOLS && i < x + radius + FLOAT_FUDGE; i++) {
        for (j = MAX(0, y - (radius + FLOAT_FUDGE)); j < DROWS && j < y + radius + FLOAT_FUDGE; j++) {
            grid[i][j] = 0;
        }
    }
    
    getFOVMask(grid, x, y, radius, T_OBSTRUCTS_VISION, (theLight->passThroughCreatures ? 0 : (HAS_MONSTER | HAS_PLAYER)),
               (!isMinersLight));
    
    overlappedFieldOfView = false;
    
    for (i = MAX(0, x - (radius + FLOAT_FUDGE)); i < DCOLS && i < x + radius; i++) {
        for (j = MAX(0, y - (radius + FLOAT_FUDGE)); j < DROWS && j < y + radius; j++) {
            if (grid[i][j]) {
                lightMultiplier = 100 - (100 - fadeToPercent) * (sqrt((i-x) * (i-x) + (j-y) * (j-y)) / radius + FLOAT_FUDGE);
                for (k=0; k<3; k++) {
                    tmap[i][j].light[k] += colorComponents[k] * lightMultiplier / 100;;
                }
                if (dispelShadows) {
                    pmap[i][j].flags &= ~IS_IN_SHADOW;
                }
                if (pmap[i][j].flags & (IN_FIELD_OF_VIEW | ANY_KIND_OF_VISIBLE)) {
                    overlappedFieldOfView = true;
                }
            }
        }
    }
    
    tmap[x][y].light[0] += colorComponents[0];
    tmap[x][y].light[1] += colorComponents[1];
    tmap[x][y].light[2] += colorComponents[2];
    
    if (dispelShadows) {
        pmap[x][y].flags &= ~IS_IN_SHADOW;
    }
    
    return overlappedFieldOfView;
}


// sets miner's light strength and characteristics based on rings of illumination, scrolls of darkness and water submersion
void updateMinersLightRadius() {
    double fraction;
    double lightRadius;
    
    lightRadius = 100 * rogue.minersLightRadius;
    
    if (rogue.lightMultiplier < 0) {
        lightRadius /= (-1 * rogue.lightMultiplier + 1);
    } else {
        lightRadius *= (rogue.lightMultiplier);
        lightRadius = MAX(lightRadius, (rogue.lightMultiplier * 2 + 2));
    }
    
    if (player.status[STATUS_DARKNESS]) {
        fraction = (double) pow(1.0 - (((double) player.status[STATUS_DARKNESS]) / player.maxStatus[STATUS_DARKNESS]), 3);
        if (fraction < 0.05) {
            fraction = 0.05;
        }
    } else {
        fraction = 1;
    }
    lightRadius = lightRadius * fraction;
    
    if (lightRadius < 2) {
        lightRadius = 2;
    }
    
    if (rogue.inWater && lightRadius > 3) {
        lightRadius = MAX(lightRadius / 2, 3);
    }
    
    rogue.minersLight.radialFadeToPercent = 35 + MAX(0, MIN(65, rogue.lightMultiplier * 5)) * (fraction + FLOAT_FUDGE);
    rogue.minersLight.lightRadius.upperBound = rogue.minersLight.lightRadius.lowerBound = CLAMP(lightRadius + FLOAT_FUDGE, -30000, 30000);
}

void updateDisplayDetail() {
    short i, j;
    
    for (i = 0; i < DCOLS; i++) {
        for (j = 0; j < DROWS; j++) {
            if (tmap[i][j].light[0] < -10
                && tmap[i][j].light[1] < -10
                && tmap[i][j].light[2] < -10) {
                
                displayDetail[i][j] = DV_DARK;
            } else if (pmap[i][j].flags & IS_IN_SHADOW) {
                displayDetail[i][j] = DV_UNLIT;
            } else {
                displayDetail[i][j] = DV_LIT;
            }
        }
    }
}

void backUpLighting(short lights[DCOLS][DROWS][3]) {
    short i, j, k;
    for (i=0; i<DCOLS; i++) {
        for (j=0; j<DROWS; j++) {
            for (k=0; k<3; k++) {
                lights[i][j][k] = tmap[i][j].light[k];
            }
        }
    }
}

void restoreLighting(short lights[DCOLS][DROWS][3]) {
    short i, j, k;
    for (i=0; i<DCOLS; i++) {
        for (j=0; j<DROWS; j++) {
            for (k=0; k<3; k++) {
                tmap[i][j].light[k] = lights[i][j][k];
            }
        }
    }
}

void recordOldLights() {
    short i, j, k;
    for (i = 0; i < DCOLS; i++) {
        for (j = 0; j < DROWS; j++) {
            for (k=0; k<3; k++) {
                tmap[i][j].oldLight[k] = tmap[i][j].light[k];
            }
        }
    }
}

void updateLighting() {
    short i, j, k;
    enum dungeonLayers layer;
    enum tileType tile;
    creature *monst;

    // Copy Light over oldLight
    recordOldLights();
    
    // and then zero out Light.
    for (i = 0; i < DCOLS; i++) {
        for (j = 0; j < DROWS; j++) {
            for (k=0; k<3; k++) {
                tmap[i][j].light[k] = 0;
            }
            pmap[i][j].flags |= IS_IN_SHADOW;
        }
    }
    
    // Paint all glowing tiles.
    for (i = 0; i < DCOLS; i++) {
        for (j = 0; j < DROWS; j++) {
            for (layer = (dungeonLayers)0; layer < NUMBER_TERRAIN_LAYERS; NEXT_LAYER(layer)) {
                tile = pmap[i][j].layers[layer];
                if (tileCatalog[tile].glowLight) {
                    paintLight(&(lightCatalog[tileCatalog[tile].glowLight]), i, j, false, false);
                }
            }
        }
    }
    
    // Cycle through monsters and paint their lights:
    CYCLE_MONSTERS_AND_PLAYERS(monst) { 
        if (monst->info.intrinsicLightType) {
            paintLight(&lightCatalog[monst->info.intrinsicLightType], monst->xLoc, monst->yLoc, false, false);
        }
        if (monst->mutationIndex >= 0 && mutationCatalog[monst->mutationIndex].light != NO_LIGHT) {
            paintLight(&lightCatalog[mutationCatalog[monst->mutationIndex].light], monst->xLoc, monst->yLoc, false, false);
        }
        
        if (monst->status[STATUS_BURNING] && !(monst->info.flags & MONST_FIERY)) {
            paintLight(&lightCatalog[BURNING_CREATURE_LIGHT], monst->xLoc, monst->yLoc, false, false);
        }
        
        if (monsterRevealed(monst)) {
            paintLight(&lightCatalog[TELEPATHY_LIGHT], monst->xLoc, monst->yLoc, false, true);
        }
    }
    
    // Also paint telepathy lights for dormant monsters.
    for (monst = dormantMonsters->nextCreature; monst != NULL; monst = monst->nextCreature) {
        if (monsterRevealed(monst)) {
            paintLight(&lightCatalog[TELEPATHY_LIGHT], monst->xLoc, monst->yLoc, false, true);
        }
    }
    
    updateDisplayDetail();
    
    // Miner's light:
    paintLight(&rogue.minersLight, player.xLoc, player.yLoc, true, true);
    
    if (player.status[STATUS_INVISIBLE]) {
        player.info.foreColor = &playerInvisibleColor;
    } else if (playerInDarkness()) {
        player.info.foreColor = &playerInDarknessColor;
    } else if (pmap[player.xLoc][player.yLoc].flags & IS_IN_SHADOW) {
        player.info.foreColor = &playerInShadowColor;
    } else {
        player.info.foreColor = &playerInLightColor;
    }
}

bool playerInDarkness() {
    return (tmap[player.xLoc][player.yLoc].light[0] + 10 < minersLightColor.red
            && tmap[player.xLoc][player.yLoc].light[1] + 10 < minersLightColor.green
            && tmap[player.xLoc][player.yLoc].light[2] + 10 < minersLightColor.blue);
}

#define flarePrecision 1000

flare *newFlare(lightSource *light, short x, short y, short changePerFrame, short limit) {
    flare *theFlare = (flare *)malloc(sizeof(flare));
    memset(theFlare, '\0', sizeof(flare));
    theFlare->light = light;
    theFlare->xLoc = x;
    theFlare->yLoc = y;
    theFlare->coeffChangeAmount = changePerFrame;
    if (theFlare->coeffChangeAmount == 0) {
        theFlare->coeffChangeAmount = 1; // no change would mean it lasts forever, which usually breaks things
    }
    theFlare->coeffLimit = limit;
    theFlare->coeff = 100 * flarePrecision;
    theFlare->turnNumber = rogue.absoluteTurnNumber;
    return theFlare;
}

// Creates a new fading flare as described and sticks it into the stack so it will fire at the end of the turn.
void createFlare(short x, short y, enum lightType lightIndex) {
    flare *theFlare;
    
    theFlare = newFlare(&(lightCatalog[lightIndex]), x, y, -15, 0);
    
    if (rogue.flareCount >= rogue.flareCapacity) {
        rogue.flareCapacity += 10;
        rogue.flares = (flare **)realloc(rogue.flares, sizeof(flare *) * rogue.flareCapacity);
    }
    rogue.flares[rogue.flareCount] = theFlare;
    rogue.flareCount++;
}

bool flareIsActive(flare *theFlare) {
    const bool increasing = (theFlare->coeffChangeAmount > 0);
    bool active = true;
    
    if (theFlare->turnNumber > 0 && theFlare->turnNumber < rogue.absoluteTurnNumber - 1) {
        active = false;
    }
    if (increasing) {
        if ((short) (theFlare->coeff / flarePrecision) > theFlare->coeffLimit) {
            active = false;
        }
    } else {
        if ((short) (theFlare->coeff / flarePrecision) < theFlare->coeffLimit) {
            active = false;
        }
    }
    return active;
}

// Returns true if the flare is still active; false if it's not.
bool updateFlare(flare *theFlare) {
    if (!flareIsActive(theFlare)) {
        return false;
    }
    theFlare->coeff += (theFlare->coeffChangeAmount) * flarePrecision / 10;
    theFlare->coeffChangeAmount = theFlare->coeffChangeAmount * 12 / 10;
    return flareIsActive(theFlare);
}

// Returns whether it overlaps with the field of view.
bool drawFlareFrame(flare *theFlare) {
    bool inView;
    lightSource tempLight = *(theFlare->light);
    color tempColor = *(tempLight.lightColor);
    
    if (!flareIsActive(theFlare)) {
        return false;
    }
    tempLight.lightRadius.lowerBound = ((long) tempLight.lightRadius.lowerBound) * theFlare->coeff / (flarePrecision * 100);
    tempLight.lightRadius.upperBound = ((long) tempLight.lightRadius.upperBound) * theFlare->coeff / (flarePrecision * 100);
    applyColorScalar(&tempColor, theFlare->coeff / flarePrecision);
    tempLight.lightColor = &tempColor;
    inView = paintLight(&tempLight, theFlare->xLoc, theFlare->yLoc, false, true);
    
    return inView;
}

// Frees the flares as they expire.
void animateFlares(flare **flares, short count) {
    short lights[DCOLS][DROWS][3];
    bool inView, fastForward, atLeastOneFlareStillActive;
    short i; // i iterates through the flare list
    
    brogueAssert(rogue.RNG == RNG_SUBSTANTIVE);
    
    backUpLighting(lights);
    fastForward = rogue.trueColorMode || rogue.playbackFastForward;
    
    do {
        inView = false;
        atLeastOneFlareStillActive = false;
        for (i = 0; i < count; i++) {
            if (flares[i]) {
                if (updateFlare(flares[i])) {
                    atLeastOneFlareStillActive = true;
                    if (drawFlareFrame(flares[i])) {
                        inView = true;
                    }
                } else {
                    free(flares[i]);
                    flares[i] = NULL;
                }
            }
        }
        demoteVisibility();
        updateFieldOfViewDisplay(false, true);
        if (!fastForward && (inView || rogue.playbackOmniscience) && atLeastOneFlareStillActive) {
            fastForward = pauseBrogue(10);
        }
        recordOldLights();
        restoreLighting(lights);
    } while (atLeastOneFlareStillActive);
    updateFieldOfViewDisplay(false, true);
}

void deleteAllFlares() {
    short i;
    for (i=0; i<rogue.flareCount; i++) {
        free(rogue.flares[i]);
    }
    rogue.flareCount = 0;
}
