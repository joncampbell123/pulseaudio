/* $Id$ */

/***
  This file is part of polypaudio.
 
  polypaudio is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published
  by the Free Software Foundation; either version 2 of the License,
  or (at your option) any later version.
 
  polypaudio is distributed in the hope that it will be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
  General Public License for more details.
 
  You should have received a copy of the GNU General Public License
  along with polypaudio; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
  USA.
***/

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <stdio.h>
#include <assert.h>

#include "sample.h"

size_t pa_frame_size(const struct pa_sample_spec *spec) {
    assert(spec);
    size_t b = 1;

    switch (spec->format) {
        case PA_SAMPLE_U8:
        case PA_SAMPLE_ULAW:
        case PA_SAMPLE_ALAW:
            b = 1;
            break;
        case PA_SAMPLE_S16LE:
        case PA_SAMPLE_S16BE:
            b = 2;
            break;
        case PA_SAMPLE_FLOAT32LE:
        case PA_SAMPLE_FLOAT32BE:
            b = 4;
            break;
        default:
            assert(0);
    }

    return b * spec->channels;
}

size_t pa_bytes_per_second(const struct pa_sample_spec *spec) {
    assert(spec);
    return spec->rate*pa_frame_size(spec);
}

uint32_t pa_bytes_to_usec(size_t length, const struct pa_sample_spec *spec) {
    assert(spec);

    return (uint32_t) (((double) length /pa_frame_size(spec))/spec->rate*1000000);
}

int pa_sample_spec_valid(const struct pa_sample_spec *spec) {
    assert(spec);

    if (!spec->rate || !spec->channels)
        return 0;

    if (spec->format >= PA_SAMPLE_MAX)
        return 0;

    return 1;
}

int pa_sample_spec_equal(const struct pa_sample_spec*a, const struct pa_sample_spec*b) {
    assert(a && b);

    return (a->format == b->format) && (a->rate == b->rate) && (a->channels == b->channels);
}

void pa_sample_snprint(char *s, size_t l, const struct pa_sample_spec *spec) {
    static const char* const table[]= {
        [PA_SAMPLE_U8] = "U8",
        [PA_SAMPLE_ALAW] = "ALAW",
        [PA_SAMPLE_ULAW] = "ULAW",
        [PA_SAMPLE_S16LE] = "S16LE",
        [PA_SAMPLE_S16BE] = "S16BE",
        [PA_SAMPLE_FLOAT32LE] = "FLOAT32LE",
        [PA_SAMPLE_FLOAT32BE] = "FLOAT32BE",
    };

    assert(pa_sample_spec_valid(spec));
    snprintf(s, l, "%s %uch %uHz", table[spec->format], spec->channels, spec->rate);
}
