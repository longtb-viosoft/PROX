/*
  Copyright(c) 2010-2016 Intel Corporation.
  All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Intel Corporation nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

struct cdf {
	uint32_t rand_max;
	uint32_t seed;
	uint32_t first_child;
	uint32_t elems[0];
};

struct cdf *cdf_create(uint32_t n_vals, int socket_id);
void cdf_add(struct cdf *cdf, uint32_t len);
int cdf_setup(struct cdf *cdf);

static uint32_t cdf_sample(struct cdf *cdf)
{
	uint32_t left_child, right_child;
	uint32_t rand;

	do {
		rand = rand_r(&cdf->seed);
	} while (rand > cdf->rand_max);

	uint32_t cur = 1;

	while (1) {
		left_child = cur * 2;
		right_child = cur * 2 + 1;
		if (right_child < cdf->elems[0])
			cur = rand > cdf->elems[cur]? right_child : left_child;
		else if (left_child < cdf->elems[0])
			cur = left_child;
		else
			return cur - cdf->first_child;
	}
}
