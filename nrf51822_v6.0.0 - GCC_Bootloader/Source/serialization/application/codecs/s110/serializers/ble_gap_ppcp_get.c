/* Copyright (c) 2013 Nordic Semiconductor. All Rights Reserved.
 *
 * The information contained herein is property of Nordic Semiconductor ASA.
 * Terms and conditions of usage are described in detail in NORDIC
 * SEMICONDUCTOR STANDARD SOFTWARE LICENSE AGREEMENT.
 *
 * Licensees are granted free, non-transferable use of the information. NO
 * WARRANTY of ANY KIND is provided. This heading must NOT be removed from
 * the file.
 *
 */

#include "ble_gap_app.h"
#include "ble_serialization.h"


uint32_t ble_gap_ppcp_get_req_enc(ble_gap_conn_params_t const * const p_conn_params,
                                  uint8_t * const                     p_buf,
                                  uint32_t * const                    p_buf_len)
{
    uint32_t index = 0;

    SER_ASSERT_NOT_NULL(p_buf);
    SER_ASSERT_NOT_NULL(p_buf_len);

    SER_ASSERT_LENGTH_LEQ(index + 1 + 1, *p_buf_len);

    p_buf[index++] = SD_BLE_GAP_PPCP_GET;
    p_buf[index++] = (p_conn_params != NULL) ? SER_FIELD_PRESENT : SER_FIELD_NOT_PRESENT;

    *p_buf_len = index;

    return NRF_SUCCESS;
}


uint32_t ble_gap_ppcp_get_rsp_dec(uint8_t const * const         p_buf,
                                  uint32_t                      packet_len,
                                  ble_gap_conn_params_t * const p_conn_params,
                                  uint32_t * const              p_result_code)
{
    uint32_t index = 0;

    uint32_t decode_result = ser_ble_cmd_rsp_result_code_dec(p_buf, &index, packet_len,
                                                             SD_BLE_GAP_PPCP_GET, p_result_code);

    if (decode_result != NRF_SUCCESS)
    {
        return decode_result;
    }

    if (*p_result_code != NRF_SUCCESS)
    {
        return NRF_SUCCESS;
    }

    SER_ASSERT_NOT_NULL(p_conn_params);

    SER_ASSERT_LENGTH_LEQ(index + 8, packet_len);
    uint16_dec(p_buf, packet_len, &index, &p_conn_params->min_conn_interval);
    uint16_dec(p_buf, packet_len, &index, &p_conn_params->max_conn_interval);
    uint16_dec(p_buf, packet_len, &index, &p_conn_params->slave_latency);
    uint16_dec(p_buf, packet_len, &index, &p_conn_params->conn_sup_timeout);

    SER_ASSERT_LENGTH_EQ(index, packet_len);

    return decode_result;
}
