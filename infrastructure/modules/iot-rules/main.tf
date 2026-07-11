locals {
  name_prefix = "${var.project_name}-${var.environment}"
}

# ── IoT Topic Rule ────────────────────────────────────────────────────────────
# Triggers on every MQTT message published to the device topic prefix.
# Extracts fields from the JSON payload and publishes a notification to SNS,
# which is delivered to subscribers (currently email).
#
# Expected device payload shape:
# {
#   "device_id": "esp32-sensor-01",
#   "timestamp": "2026-07-10T11:00:00Z",
#   "event":     "motion_detected",
#   "ttl":       1757000000        (Unix epoch, optional)
# }

resource "aws_iot_topic_rule" "sensor_to_sns" {
  name        = replace("${local.name_prefix}_sensor_data", "-", "_")
  description = "Routes sensor MQTT messages to SNS for email delivery"
  enabled     = true

  # SQL selects the full message plus the topic for traceability
  sql         = "SELECT *, topic() AS mqtt_topic FROM '${var.topic_prefix}/+'"
  sql_version = "2016-03-23"

  sns {
    role_arn   = var.iot_rule_role_arn
    target_arn = var.sns_topic_arn

    # Email subject line — pulls the event and device id straight from the payload
    message_format = "RAW"

    # The full JSON payload becomes the email body. Using STRING keeps the JSON
    # human-readable in the email client.
  }

  # Dead-letter queue: if the SNS action fails, log the error to CloudWatch
  error_action {
    cloudwatch_logs {
      log_group_name = aws_cloudwatch_log_group.iot_rule_errors.name
      role_arn       = var.iot_rule_role_arn
    }
  }

  tags = {
    Name = "${local.name_prefix}-sensor-rule"
  }
}

# ── CloudWatch Log Group for rule errors ──────────────────────────────────────

resource "aws_cloudwatch_log_group" "iot_rule_errors" {
  name              = "/aws/iot/${local.name_prefix}/rule-errors"
  retention_in_days = 14

  tags = {
    Name = "${local.name_prefix}-iot-rule-errors"
  }
}
